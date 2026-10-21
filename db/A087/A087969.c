/*
 * A087969  Integers m such that gcd(2^m+1, m^2) is not a square.
 * https://oeis.org/A087969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 2446, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087969: integers m such that gcd(2^m+1, m^2) is not a square.
   Implementation searches m in increasing order and caches the results.
   Existing helper functions: gcd, floor_sqrt, mod_pow.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms    = NULL; /* array of m values */
    static size_t term_count = 0;  /* number of terms stored */
    static size_t capacity    = 0;  /* allocated size of terms[] */
    static idx_t last_checked = 0;  /* largest m processed so far */

    /* if we already have the n‑th term, return it */
    if (n <= term_count)
        return terms[n - 1];

    /* produce terms until the cache holds at least n of them */
    while (term_count < n)
    {
        last_checked++;
        idx_t m = last_checked;            /* candidate integer m   */
        seq_t mm = (seq_t)m;                /* convert to seq_t       */

        /* m² */
        seq_t mod = mm * mm;

        /* 2^m mod m²  */
        seq_t r = mod_pow((seq_t)2, m, mod);

        /* 2^m + 1  (then take mod m² again) */
        r = r + (seq_t)1;
        r = r % mod;

        /* gcd(2^m+1, m²) */
        seq_t g = gcd(r, mod);

        /* check whether g is a perfect square */
        seq_t sq = floor_sqrt(g);
        if (sq * sq != g)
        {
            /* append m to the cache */
            if (term_count == capacity)
            {
                capacity = capacity ? capacity * 2 : 4;
                terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
            }
            terms[term_count++] = mm;
        }
    }

    return terms[n - 1];
}
