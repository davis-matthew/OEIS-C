/*
 * A235271  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-2) - 24*a(n-4) for n>=5; a(1)=40, a(2)=100,
 *            a(3)=208, a(4)=520
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values (n >= 1) */
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)40;
            case 2: return (seq_t)100;
            case 3: return (seq_t)208;
            case 4: return (seq_t)520;
        }
    }

    /* Compute up to n using the recurrence */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq) return (seq_t)0;  /* allocation failed – return 0 */

    seq[1] = 40;
    seq[2] = 100;
    seq[3] = 208;
    seq[4] = 520;

    for (idx_t i = 5; i <= n; ++i) {
        seq[i] = 10 * seq[i - 2] - 24 * seq[i - 4];
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
