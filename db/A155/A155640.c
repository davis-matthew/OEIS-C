/*
 * A155640  a(n) = 7^n - 5^n + 1^n.
 * https://oeis.org/A155640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 5^n + 1^n (equivalently, a(n) =
 *            12*a(n-1)-35*a(n-2)+24)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 7^n and 5^n using the available big‑power routine.  */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);

    /* 1^n is always 1, so add it directly.  */
    return pow7 - pow5 + (seq_t)1;
}
