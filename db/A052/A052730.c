/*
 * A052730  A simple context-free grammar in a labeled universe: labeled
 *          version of A052703.
 * https://oeis.org/A052730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A052730: a(n) = n!*A052703(n)
    D-finite recurrence (valid for n ≥ 2):
      (-105*n+135*n^3+30)*a(n)  + (-130-107*n+29*n^2)*a(n+1)
    + (-99-91*n)*a(n+2) + 15*a(n+3) = 0  (a(1)=0, a(2)=0, a(3)=6, a(4)=72) */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;   /* array of a(0)..a(max)  */
    static size_t  cap     = 0;     /* current allocation size   */
    static idx_t   comp    = 0;     /* highest index computed   */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* at least hold indices 0..4 */
        cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;        /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)72;
        comp = 4;                           /* a(0)..a(4) are known */
    }

    /* grow cache if required */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    if (n > comp) {
        for (idx_t k = comp + 1; k <= n; ++k) {
            if (k <= 4) continue; /* already set */

            /* recurrence uses index m = k-3  (since a(k) depends on a(k-3..k-1)) */
            idx_t m = k - 3;

            /* coefficients (signed 64-bit to hold negatives) */
            int64_t cn   = -105 * (int64_t)m + 135 * (int64_t)m * m * m + 30;
            int64_t cn1  = -130 - 107 * (int64_t)m + 29 * (int64_t)m * m;
            int64_t cn2  = -99 - 91 * (int64_t)m;

            seq_t pos = 0;   /* sum of products with positive coefficients   */
            seq_t neg = 0;   /* sum of products with negative coefficients   */

            /* a(k-3) term */
            if (cn >= 0) {
                pos += (seq_t)cn * cache[k-3];
            } else {
                neg += (seq_t)(-cn) * cache[k-3];
            }

            /* a(k-2) term */
            if (cn1 >= 0) {
                pos += (seq_t)cn1 * cache[k-2];
            } else {
                neg += (seq_t)(-cn1) * cache[k-2];
            }

            /* a(k-1) term */
            if (cn2 >= 0) {
                pos += (seq_t)cn2 * cache[k-1];
            } else {
                neg += (seq_t)(-cn2) * cache[k-1];
            }

            /* numerator = neg - pos  (always non‑negative when the sequence is correct) */
            seq_t numer = neg - pos;

            /* a(k) = numer / 15  */
            cache[k] = numer / (seq_t)15;
        }
        comp = n;
    }

    return cache[n];
}
