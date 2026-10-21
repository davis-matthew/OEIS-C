/*
 * A063235  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          83 ).
 * https://oeis.org/A063235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) for n ≥ 7, with initial terms
 *            a(1)=7, a(2)=20, a(3)=34, a(4)=48, a(5)=62, a(6)=74
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    static seq_t *seq = NULL;        /* cache of computed terms */
    static idx_t cached = 0;         /* highest n computed so far */

    /* Ensure we can index up to n */
    if (n > cached) {
        size_t newSize = (size_t)n + 1;
        if (newSize < 7) newSize = 7; /* at least up to a(6) */

        /* First allocation or size increase */
        if (!seq) {
            seq = (seq_t *)malloc(newSize * sizeof(seq_t));
            if (!seq) return 0;       /* allocation failed */

            /* initialise known initial values */
            seq[0] = 0;               /* dummy for 1‑based indexing */
            seq[1] = (seq_t)7;
            seq[2] = (seq_t)20;
            seq[3] = (seq_t)34;
            seq[4] = (seq_t)48;
            seq[5] = (seq_t)62;
            seq[6] = (seq_t)74;
            cached = 6;
        } else {
            seq = (seq_t *)realloc(seq, newSize * sizeof(seq_t));
            if (!seq) return 0;       /* allocation failed */
        }

        /* Compute all new terms up to n */
        for (idx_t i = cached + 1; i <= n; ++i) {
            /* recurrence holds for i ≥ 7; earlier values already set */
            if (i <= 6) continue;      /* values up to a(6) already known */
            seq[i] = seq[i - 2] + seq[i - 3] - seq[i - 5];
        }
        cached = n;
    }

    return seq[n];
}
