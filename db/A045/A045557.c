/*
 * A045557  Numbers whose factorial has '32' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 3250, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A045557: Numbers whose factorial has '32' just before the trailing zeros.
   Computation method: iterate integers m, maintain the value of 
   m! with all trailing zeros removed modulo 100.  When this value equals 32,
   m is appended to the result list.  The search is memoised so that
   any call to a(n) completes in O(max m for n) time.
 */

#include <stdlib.h>
#include <stdint.h>

/* power of 2 modulo 100, exponent may be large */
static seq_t
pow2mod(uint64_t exp)          /* use unsigned long long for safety */
{
    seq_t result = 1;
    seq_t base   = 2;
    uint64_t e   = exp;
    while (e) {
        if (e & 1)
            result = (seq_t)((unsigned long long)result * base % 100);
        base = (seq_t)((unsigned long long)base * base % 100);
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;      /* array of found terms */
    static size_t  cache_cap = 0;         /* allocated capacity */
    static size_t  cache_len = 0;         /* number of terms stored */
    static idx_t   checked   = 0;         /* largest m tested so far */

    static seq_t   prod  = 1;             /* product of numbers with 2s & 5s removed */
    static idx_t   c2    = 0;             /* count of removed 2s   */
    static idx_t   c5    = 0;             /* count of removed 5s   */

    /* make sure we have at least n terms */
    while (cache_len < n) {
        checked++;                      /* next integer to test */
        idx_t x = checked;

        /* remove all factors of 5, increment c5 */
        while (x % 5 == 0) {
            x /= 5;
            c5++;
        }
        /* remove all factors of 2, increment c2 */
        while (x % 2 == 0) {
            x /= 2;
            c2++;
        }

        /* update product modulo 100 */
        unsigned long long tmp = (unsigned long long)prod * (x % 100);
        prod = (seq_t)(tmp % 100);

        /* compute last two digits before trailing zeros */
        idx_t diff = c2 - c5;           /* surplus 2's after cancelling 5's */
        seq_t last_two = (seq_t)((unsigned long long)prod * pow2mod(diff) % 100);

        if (last_two == 32) {
            /* store the term */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmpp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmpp) exit(1);      /* out of memory */
                cache = tmpp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)checked;
        }
    }

    return cache[n - 1];  /* zero-based indexing */
}
