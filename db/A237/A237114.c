/*
 * A237114  Smallest semiprime of the form k^prime(n)+1, or 0 if no such
 *          semiprime exists.
 * https://oeis.org/A237114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   ipow, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237114:  Smallest semiprime of the form k^prime(n)+1,
 * with both k+1 and (k^prime(n)+1)/(k+1) prime, except at n=1
 * where the restriction on k+1 is dropped.
 *
 * This routine computes the term by brute force search for k,
 * using the helper functions:
 *   get_prime   : retrieve the n-th prime
 *   big_pow     : exact exponentiation
 *   is_prime_big: primality test for arbitrary width integers
 *
 * Complexity is dominated by the big_pow call for the smallest
 * valid k, which for most n is k=2, so per call is very fast.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow.h"
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

seq_t a(idx_t n)
{
    /* Handle the exceptional offset n==1 */
    if (n == 1) {
        /* Find the smallest k such that k^2+1 is a semiprime */
        for (uint64_t k = 2; ; ++k) {
            seq_t val = (seq_t)k * (seq_t)k + 1;
            if (is_prime_big(val))
                continue;                       /* skip if prime */
            /* trial division up to sqrt(val) */
            for (seq_t d = 2; d * d <= val; ++d) {
                if (val % d == 0) {
                    seq_t other = val / d;
                    if (is_prime_big(d) && is_prime_big(other))
                        return val;
                }
            }
        }
    }

    /* n >= 2: standard definition */
    seq_t p = (seq_t)get_prime((idx_t)n);       /* p = prime(n)  */
    for (uint64_t k = 2; ; ++k) {
        seq_t k1 = (seq_t)k + 1;                /* k+1       */
        if (!is_prime_big(k1))
            continue;                           /* need k+1 prime */

        seq_t val = ipow((seq_t)k, p) + 1;   /* k^p + 1   */
        if (val % k1 != 0)
            continue;                           /* division must be exact */

        seq_t q = val / k1;                     /* the other factor */
        if (is_prime_big(q))
            return val;                         /* semiprime found */
    }
}
