/*
 * A029051  Expansion of 1/((1-x)(1-x^3)(1-x^7)(1-x^10)).
 * https://oeis.org/A029051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-3)-a(n-4)+a(n-7)-a(n-8)-a(n-13)+a(n-14)-a(n-17)+a(n-18)+a(n-20)-a(n-21)
 *            Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=2,
 *            a(6)=3, a(7)=4, a(8)=4, a(9)=5, a(10)=7, a(11)=7, a(12)=8,
 *            a(13)=10, a(14)=11, a(15)=12, a(16)=14, a(17)=16, a(18)=17,
 *            a(19)=19, a(20)=22
 * Range:     u8 n <= 58, u16 n <= 424, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[21] = {
    1, 1, 1, 2, 2, 2, 3, 4, 4, 5,
    7, 7, 8,10,11,12,14,16,17,19,22
};

static seq_t *dp = NULL;      /* cache of computed terms  */
static idx_t dp_size = 0;     /* number of elements stored   */

seq_t a(idx_t n)
{
    if (dp_size == 0) {
        /* first call – initialise cache to at least 21 elements   */
        dp_size = 21;
        dp = (seq_t *)malloc((size_t)dp_size * sizeof(seq_t));
        for (idx_t i = 0; i < 21; ++i)
            dp[(size_t)i] = init_vals[(size_t)i];
    }

    if (n >= dp_size) {
        /* grow cache to accommodate index n (at least n+1)   */
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_dp) exit(1);   /* allocation failed – terminate */

        /* copy old values */
        for (idx_t i = 0; i < dp_size; ++i)
            new_dp[(size_t)i] = dp[(size_t)i];
        free(dp);
        dp = new_dp;

        /* initialise any new elements between old size and new_size */
        for (idx_t i = dp_size; i < new_size; ++i) {
            if (i < 21) {
                dp[(size_t)i] = init_vals[(size_t)i];
            } else {
                /* recurrence: indices are guaranteed to exist */
                seq_t term = dp[(size_t)(i-1)] + dp[(size_t)(i-3)]
                           - dp[(size_t)(i-4)] + dp[(size_t)(i-7)]
                           - dp[(size_t)(i-8)] - dp[(size_t)(i-13)]
                           + dp[(size_t)(i-14)] - dp[(size_t)(i-17)]
                           + dp[(size_t)(i-18)] + dp[(size_t)(i-20)]
                           - dp[(size_t)(i-21)];
                dp[(size_t)i] = term;
            }
        }
        dp_size = new_size;
    }

    return dp[(size_t)n];
}
