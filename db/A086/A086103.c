/*
 * A086103  Primes appearing as the concatenation of the last two digits of
 *          prime(A086102(n)) and the first two digits of prime(A086102(n)+1).
 * https://oeis.org/A086103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A086103: primes that are concatenation of
   last two digits of prime(k) and first two digits of prime(k+1). */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

/* Utilities provided by the framework */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

#define MAX_CACHE 20000  /* safe upper bound for n <= ~10000 */

static seq_t cache[MAX_CACHE];   /* memoised a(n) values */
static size_t cache_cnt = 0;     /* number of elements stored */
static idx_t  gen_k   = 0;       /* current prime index processed (pair (k,k+1)) */

/* Return the nth term of the OEIS sequence A086103.  n >= 1.  */
seq_t a(idx_t n)
{
    /* Ensure we have at least n elements generated. */
    while (cache_cnt < n) {
        /* Process the next consecutive prime pair. */
        ++gen_k;                                 /* current prime index k */
        seq_t p_k    = get_prime(gen_k);          /* prime_k   */
        seq_t p_next = get_prime(gen_k + 1);      /* prime_{k+1} */

        /* Last two digits of p_k. */
        seq_t L = p_k % (seq_t)100;

        /* First two digits of p_next. */
        int dnext = num_digits(p_next); /* number of decimal digits of p_next */
        seq_t F;
        if (dnext > 1) {
            /* p_next has at least two digits. */
            seq_t pow = pow10(dnext - 2);
            F = p_next / pow;
        } else {
            /* p_next is a one–digit prime. */
            F = p_next;
        }

        /* Concatenated number: last two digits of p_k followed by first two of p_next. */
        seq_t concat = L * (seq_t)100 + F;

        /* If the concatenated number is prime, store it. */
        if (is_prime_big(concat)) {
            if (cache_cnt < MAX_CACHE) {
                cache[cache_cnt++] = concat;
            } else {
                /* Exceeded preallocated cache – should not happen for n <= ~10000. */
                /* If it does, we simply break; subsequent requests will fail. */
                break;
            }
        }
    }
    return cache[n - 1];
}
