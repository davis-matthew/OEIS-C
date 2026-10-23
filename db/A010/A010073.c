/*
 * A010073  a(n) = sum of base-6 digits of a(n-1) + sum of base-6 digits of
 *          a(n-2); a(0)=0, a(1)=1.
 * https://oeis.org/A010073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   digit_sum_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum_base6.h"
/* Recurrence: a(0)=0, a(1)=1,
 * a(n)= digit_sum_base6(a(n-1)) + digit_sum_base6(a(n-2))   (n≥2)   */
/* Digit‑sum in base 6:  s(x)=∑_{k≥0} (x÷6ᵏ mod 6)            */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;  /* a(n-2)  */
    seq_t prev1 = (seq_t)1;  /* a(n-1)  */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t s0 = digit_sum_base6(prev2);
        seq_t s1 = digit_sum_base6(prev1);
        cur = s0 + s1;

        prev2 = prev1;
        prev1 = cur;
    }

    return cur;   /* equals a(n) */
}
