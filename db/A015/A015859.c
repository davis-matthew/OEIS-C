/*
 * A015859  Numbers k such that phi(k) | sigma(k + 12).
 * https://oeis.org/A015859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 260, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A015859: numbers k such that φ(k) | σ(k + 12)
   Formula: the n‑th term is the smallest integer k satisfying that
   condition, enumerated in increasing order.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- Helper functions for uint64_t arithmetic ---------- */

/* Euler's totient φ(n) for n up to 64 bits, using trial division. */
static uint64_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t original = n;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            result -= result / p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

/* Sum of divisors σ(n) for n up to 64 bits, using trial division. */
static uint64_t sigma_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t term = 1;
            uint64_t power = 1;
            while (m % p == 0) {
                m /= p;
                power *= p;
                term += power;
            }
            result *= term;
        }
    }
    if (m > 1) result *= (1 + m);  /* remaining prime factor */
    return result;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* The first term of the sequence is defined for n >= 1. */
    if (n == 0) return (seq_t)0;

    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t checked = 0;       /* largest k that has been examined */

    /* Initialise cache. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);   /* memory allocation failure */
    }

    /* Keep searching until we have at least n terms. */
    while (cache_len < (size_t)n) {
        ++checked;                   /* next integer k to test */

        /* Convert checked to seq_t for return, but use uint64_t for arithmetic. */
        uint64_t k = checked;
        uint64_t phi_k   = phi_uint64(k);
        uint64_t sigma_k = sigma_uint64(k + 12ULL);

        /* φ(k) cannot be zero for k > 0.  Check divisibility. */
        if (phi_k != 0 && sigma_k % phi_k == 0) {
            /* Append to cache, resizing if necessary. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) exit(1);
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
