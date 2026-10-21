/*
 * A224426  The hyper-Wiener index of the phenylacetylene dendrimer NSB[n],
 *          defined pictorially in the Z. Yarahmadi et al. reference.
 * https://oeis.org/A224426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 18, u128 n <= 48,
 *            big any n
 * Helpers:   _pow2_small, _pow4_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence OEIS A224426:
   Hyper-Wiener index of the phenylacetylene dendrimer NSB[n].
   Defined by the formula:
   a(n) = 116340 - 2180388*2^n + 2132604*4^n
          - 2299671*4^n*n/2 + 613089*4^n*n^2
   (The generating function is 6*(11426+185109*x+295053*x^2+31342*x^3+600*x^4)/
   ((1-x)*(1-2*x)*(1-4*x)^3) leading to the linear recurrence
   a(n) = 15*a(n-1) - 86*a(n-2) + 232*a(n-3) - 288*a(n-4) + 128*a(n-5)
   for n ≥ 5).
   The implementation caches computed values so repeated calls are O(1).
   The first five terms are computed directly with the closed form. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/_pow2_small.h"
#include "../utils/_pow4_small.h"

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *buf = NULL;
    static size_t cap = 0;          /* current allocated size */

    /* Expand cache if needed */
    if ((size_t)n >= cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!new_buf) return 0;    /* allocation failure – unlikely in tests */
        buf = new_buf;

        /* Compute new elements */
        for (size_t i = cap; i < new_cap; ++i)
        {
            if (i < 5)  /* use closed form for first 5 terms */
            {
                seq_t pow2n = _pow2_small((idx_t)i);
                seq_t pow4n = _pow4_small((idx_t)i);
                seq_t term2 = 2180388 * pow2n;
                seq_t term3 = 2132604 * pow4n;

                seq_t t4 = 2299671 * pow4n;
                t4 *= (seq_t)i;
                t4 /= 2;

                seq_t t5 = 613089 * pow4n;
                t5 *= (seq_t)i;
                t5 *= (seq_t)i;

                buf[i] = 116340 - term2 + term3 - t4 + t5;
            }
            else  /* use the linear recurrence for n >= 5 */
            {
                buf[i] = 15 * buf[i-1]
                       - 86 * buf[i-2]
                       +232 * buf[i-3]
                       -288 * buf[i-4]
                       +128 * buf[i-5];
            }
        }
        cap = new_cap;
    }

    return buf[(size_t)n];
}
