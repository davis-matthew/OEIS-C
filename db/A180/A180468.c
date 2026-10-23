/*
 * A180468  Numbers m such that m/k is an integer. m=(x_1 x_2 ... x_r) where
 *          x_i are digits of m, k = x_1^r + x_2^(r-1) + ... + x_r^1.
 * https://oeis.org/A180468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) computes the n-th smallest integer m such that m/(x1^r +
 *            x2^(r‑1)+ … + xr^1) is an integer, where the digits of m are
 *            x1…xr. The sequence starts 0,1,2,3,4,5,6,7,8,9,10,12,… and is
 *            generated in ascending order. The implementation caches already
 *            found terms so that repeated calls are fast
 * Range:     u8 n <= 38, u16 n <= 292, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* static state for memoization */
static seq_t *cache = NULL;      /* array holding found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity of the array */
static uint64_t tested = 0;      /* next integer to test for the property */

/* grow the cache storage to at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);          /* out of memory */
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* computes the n'th term of A180468 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* unused, but safe */

/* already produced enough terms? */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

/* generate new terms until we have at least n */
    seq_t m = tested;                /* start from last tested value */

    for ( ; cache_len < (size_t)n; ++m ) {
        if (m == 0) {                /* the number 0 is included by definition */
            grow_cache(cache_len + 1);
            cache[cache_len++] = 0;
            continue;
        }

        /* extract decimal digits of m into array 'digits'.
           Digits[0] is least significant digit. */
        seq_t tmp = m;
        uint8_t digits[64];
        size_t r = 0;
        while (tmp != 0) {
            digits[r++] = (uint8_t)(tmp % 10);
            tmp = tmp / 10;
        }

        /* compute k = x1^r + x2^(r-1) + … + xr^1.
           digits[i] is the (i+1)-th least significant digit,
           so its exponent is i+1. */
        seq_t k = 0;
        for (size_t i = 0; i < r; ++i) {
            seq_t base = digits[i];
            seq_t powv  = 1;
            for (size_t j = 0; j <= i; ++j) powv *= base;   /* exponent = i+1 */
            k += powv;
        }

        if (k != 0 && m % k == 0) {   /* m/k is integer */
            grow_cache(cache_len + 1);
            cache[cache_len++] = m;
        }
    }

    /* remember where we stopped; next call will continue from here */
    tested = (uint64_t)m;           /* m has already been incremented by the for-loop */

    return cache[n - 1];
}
