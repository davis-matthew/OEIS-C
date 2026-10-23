/*
 * A169573  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A169573: Number of reduced words of length n in a Coxeter group 
   on 32 generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.

   The generating function yields the linear recurrence

       a(n) - 30·a(n-1) - 30·a(n-2) - … - 30·a(n-34) + 465·a(n-35) = p(n),

   where

       p(0) = 1,
       p(n) = 2          for 1 ≤ n ≤ 34,
       p(35) = 1,
       p(n) = 0          for n > 35.

   This implementation evaluates the recurrence directly, keeping a circular
   buffer of the last 35 terms.  All arithmetic is performed with the type
   seq_t, which may be an unsigned 8/16/32/64‑bit integer or an arbitrary‑
   precision integer; therefore only +, -, * and small integer constants
   are used.

   The algorithm runs in Θ(n) time and uses O(1) additional space.
*/

seq_t a(idx_t n)
{
    /* Circular buffer of the last 35 computed terms, initialised to zero. */
    static seq_t buf[35];
    for (idx_t i = 0; i < 35; ++i)
        buf[i] = 0;

    seq_t cur = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* Inhomogeneous term p(i). */
        seq_t pi;
        if (i == 0)
            pi = 1;
        else if (i <= 34)
            pi = 2;
        else if (i == 35)
            pi = 1;
        else
            pi = 0;

        /* Compute a(i) = pi + Σ_{k=1}^{min(34,i)} 30·a(i-k) - 465·a(i-35). */
        seq_t sum = pi;

        idx_t up = (i < 34 ? i : 34);
        for (idx_t k = 1; k <= up; ++k)
            sum += 30 * buf[(i - k) % 35];

        if (i >= 35)
            sum -= 465 * buf[(i - 35) % 35];

        cur = sum;
        buf[i % 35] = cur;
    }

    return cur;
}
