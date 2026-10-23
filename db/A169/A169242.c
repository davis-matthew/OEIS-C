/*
 * A169242  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A169242
   Formula:
     a(0) = 1;
     for 1 ≤ n ≤ 27:   a(n) = 2 + 35 * Σ_{k=0}^{n-1} a(k);
     for n ≥ 28:       a(n) = 35 * Σ_{k=1}^{27} a(n-k) - 630 * a(n-28);
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    const seq_t ONE   = (seq_t)1;
    const seq_t TWO   = (seq_t)2;
    const seq_t C35   = (seq_t)35;
    const seq_t C630  = (seq_t)630;

    if (n == 0)
        return ONE;

    /* Compute the first 28 terms explicitly */
    seq_t buf[28] = {0};
    seq_t sum_c   = ONE;      /* sum of a[0]..a[i-1] during construction */
    buf[0] = ONE;

    for (idx_t i = 1; i <= (idx_t)27 && i <= n; ++i)
    {
        buf[(size_t)i] = TWO + C35 * sum_c;
        sum_c += buf[(size_t)i];
    }

    if (n <= (idx_t)27)
        return buf[(size_t)n];

    /* Now n ≥ 28.  buf[0..27] holds a[0]..a[27]. */
    seq_t sum_last27 = sum_c - buf[0];   /* Σ_{k=1}^{27} a(k) */

    for (idx_t i = (idx_t)28; i <= n; ++i)
    {
        idx_t idxA      = (i - (idx_t)28) % (idx_t)28;
        idx_t idxLeave  = (i - (idx_t)27) % (idx_t)28;

        seq_t current = C35 * buf[(size_t)idxA] - C630 * sum_last27;
        buf[(size_t)(i % (idx_t)28)] = current;

        /* Update the sliding-window sum for the next iteration. */
        sum_last27 = sum_last27 - buf[(size_t)idxLeave] + current;
    }

    return buf[(size_t)(n % (idx_t)28)];
}
