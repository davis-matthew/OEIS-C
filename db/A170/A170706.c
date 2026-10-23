/*
 * A170706  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170706:
   a(0) = 1
   For n ≥ 1:
      a(n) = N_n + 23 * Σ_{i=1}^{49} a(n – i) – 276 * a(n – 50)
   where                N_0 = 1,
                        N_k = 2 for 1 ≤ k ≤ 49,
                        N_50 = 1,
                        N_k = 0 for k > 50.  */

#include <stdint.h>   /* for fixed-width integer types, if needed */

/*  seq_t and idx_t are defined by the build system.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* circular buffer holding the last 50 terms          */
    seq_t ring[50];
    for (idx_t i = 0; i < 50; ++i)
        ring[i] = (seq_t)0;
    ring[0] = (seq_t)1;                        /* a(0) */
    seq_t window_sum = (seq_t)1;                /* Σ a(0) */
    seq_t a_n = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* coefficient N_i */
        seq_t N = (i == 50) ? (seq_t)1
                         : ((i <= 49) ? (seq_t)2 : (seq_t)0);

        /* a(i-50) contributes only when i ≥ 50 */
        seq_t prev50 = (i >= 50) ? ring[(i - 50) % 50] : (seq_t)0;

        /* recurrence */
        a_n = N + ((seq_t)23 * window_sum) - ((seq_t)276 * prev50);

        /* store current term in the circular buffer */
        ring[i % 50] = a_n;

        /* update the window sum for the next step:
           add a(i), subtract a(i-49) if the window size exceeds 49  */
        window_sum += a_n;
        if (i >= 49)
        {
            seq_t out = ring[(i - 49) % 50];   /* a(i-49) */
            window_sum -= out;
        }
    }

    return a_n;   /* a(n) for n ≥ 1 */
}
