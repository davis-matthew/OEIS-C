/*
 * A189940  Number of connected components in all simple labeled graphs with n
 *          nodes having degrees at most one.
 * https://oeis.org/A189940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of cycles in all involutions of {1..n}
 *            Recurrence: let I(0)=1, I(1)=1, I(k)=I(k-1)+(k-1)*I(k-2); let
 *            C(0)=0, C(1)=1, C(k)=C(k-1)+I(k-1)+(k-1)*(C(k-2)+I(k-2));
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined for n=0, but safe to return 0 */
    if (n == 1) return 1;   /* a(1) = 1 */

    seq_t I_prev2 = 1; /* I(0) */
    seq_t I_prev1 = 1; /* I(1) */
    seq_t C_prev2 = 0; /* C(0) */
    seq_t C_prev1 = 1; /* C(1) */

    seq_t I_curr, C_curr;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t coeff = (seq_t)(i - 1);
        I_curr = I_prev1 + coeff * I_prev2;
        C_curr = C_prev1 + I_prev1 + coeff * (C_prev2 + I_prev2);
        I_prev2 = I_prev1;
        I_prev1 = I_curr;
        C_prev2 = C_prev1;
        C_prev1 = C_curr;
    }

    return C_prev1;
}
