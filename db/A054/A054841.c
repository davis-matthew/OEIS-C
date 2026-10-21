/*
 * A054841  If n = 2^a * 3^b * 5^c * 7^d * ... then a(n) = a + 10 * b + 100 *
 *          c + 1000 * d + ... .
 * https://oeis.org/A054841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{i>0} e_i·10^(i‑1) when n = Π_{i>0} prime(i)^e_i
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 30, u64 n <= 72, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054841 – “exponential prime power representation” */

/* The following implementation uses only the language features required
 * in the specification (C11 / C++17).  It contains its own lightweight
 * prime generator and factoriser, no external utilities are needed.
 * The function works for all n ≥ 1 and is O(√n) per call.  It is
 * sufficient for the typical limits of this sequence (n up to about
 * 10 000 or more). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_list  = NULL;   /* dynamic array of primes */
static size_t   prime_count = 0;       /* number of primes stored */
static size_t   prime_cap   = 0;       /* allocated capacity */

/* ------------------------------------------------------------------ */
/* Integer square‑root:  floor(sqrt(n))                               */
/* ------------------------------------------------------------------ */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid == 0) { low = 1; continue; }
        if (mid <= n / mid) {            /* mid*mid <= n  */
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Ensure that prime_list contains all primes up to 'limit'.           */
/* This function extends the list incrementally (no full sieve).        */
/* ------------------------------------------------------------------ */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit < 2) return;

    /* If we already have all needed primes, nothing to do.  */
    if (prime_count > 0 && prime_list[prime_count - 1] >= limit) return;

    /* Grow capacity as needed (doubling strategy).  */
    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else {
        while (prime_cap <= limit) {
            prime_cap <<= 1;
        }
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }

    uint64_t start = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    for (uint64_t candidate = start; candidate <= limit; ++candidate) {
        bool is_prime = true;
        uint64_t sqrt_c = isqrt_uint64(candidate);
        for (size_t i = 0; i < prime_count && prime_list[i] <= sqrt_c; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = candidate;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Binary search: first index such that prime_list[idx] >= value.      */
/* ------------------------------------------------------------------ */
static size_t lower_bound_prime(uint64_t value)
{
    size_t low = 0, high = prime_count;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (prime_list[mid] < value)
            low = mid + 1;
        else
            high = mid;
    }
    return low;   /* could be prime_count if value > all primes */
}

/* ------------------------------------------------------------------ */
/* 10^e, computed in seq_t.                                           */
/* ------------------------------------------------------------------ */
static inline seq_t pow10_idx(size_t exp)   /* exp = i-1 (i starts at 1)      */
{
    seq_t r = 1;
    while (exp--) r *= 10;
    return r;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* indices start at 1; the sequence is defined only for n>=1. */
    if (n < 2) return 0;   /* a(1) = 0 */

    uint64_t n0 = (uint64_t)n;
    uint64_t temp = n0;
    seq_t   result = 0;

    /* Factorise n using incremental prime generation.  */
    uint64_t limit = isqrt_uint64(temp);
    ensure_primes_upto(limit);

    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint64_t p = prime_list[idx];
        if (p > limit) break;
        if (temp % p == 0) {
            /* Count exponent of prime p. */
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* Weight for this prime: 10^(idx)  (idx starts at 0 for prime 2). */
            seq_t weight = pow10_idx(idx);        /* idx = i-1 */
            result += (seq_t)exp * weight;
        }
    }

    /* If temp > 1, it is a prime factor larger than sqrt(original n). */
    if (temp > 1) {
        /* Ensure primes up to temp are generated to know its index. */
        ensure_primes_upto(temp);
        size_t pos = lower_bound_prime(temp);
        if (pos >= prime_count || prime_list[pos] != temp) {
            /* temp is a prime not stored yet (possible only if temp > previous limit). */
            /* Append it. */
            if (prime_count == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = temp;
            pos = prime_count - 1;
        }
        seq_t weight = pow10_idx(pos);   /* pos == i-1 */
        result += weight;   /* exponent e = 1 */
    }

    return result;
}
