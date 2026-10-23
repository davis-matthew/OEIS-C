/*
 * A041158  Numerators of continued fraction convergents to sqrt(89).
 * https://oeis.org/A041158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   Let a(0) = 9. For n ≥ 1, let the partial quotients
 *            (continued‑fraction terms) of √89 be q(n) = { 2, 3, 3, 2, 18 }
 *            repeating every 5 terms. Define the convergent numerators h_n by
 *            h_{-2} = 0, h_{-1} = 1 h_n = q(n) * h_{n-1} + h_{n-2} (n ≥ 0)
 *            Then a(n) = h_n. This reproduces the OEIS sequence A041158
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* h_{-2} and h_{-1} */
    seq_t h_prev2 = 0;   /* h_{-2} */
    seq_t h_prev1 = 1;   /* h_{-1} */
    seq_t h_cur;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t q;
        if (i == 0)
            q = (seq_t)9;
        else
        {
            idx_t r = (i - 1) % 5;        /* 0 .. 4 */
            switch (r)
            {
                case 0: q = (seq_t)2; break;
                case 1: q = (seq_t)3; break;
                case 2: q = (seq_t)3; break;
                case 3: q = (seq_t)2; break;
                default: q = (seq_t)18;   /* r == 4 */
            }
        }
        h_cur = q * h_prev1 + h_prev2;
        h_prev2 = h_prev1;
        h_prev1 = h_cur;
    }
    return h_prev1;   /* h_n for the requested n */
}
