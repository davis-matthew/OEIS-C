/*
 * A052268  First differences of 10^n (A011557).
 * https://oeis.org/A052268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*10^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* All n ≥ 1 (offset 1).  For n = 0 the sequence is not defined,
       but we return 0 to avoid undefined behaviour. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 10^(n-1) exactly using the pre‑built power‑of‑10 routine. */
    seq_t ten_pow = pow10(n - 1);

    /* Multiply by 9 to obtain a(n).  All intermediate values fit in seq_t. */
    return ten_pow * (seq_t)9;
}
