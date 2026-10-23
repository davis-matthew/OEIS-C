/*
 * A155604  a(n) = 6^n + 3^n - 1.
 * https://oeis.org/A155604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + 3^n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* helper: exponentiation by squaring for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow6 = pow_seq((seq_t)6, n);
    seq_t pow3 = pow_seq((seq_t)3, n);
    return pow6 + pow3 - (seq_t)1;
}
