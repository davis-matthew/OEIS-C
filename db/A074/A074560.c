/*
 * A074560  a(n) = 3^n + 8^n + 9^n.
 * https://oeis.org/A074560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 8^n + 9^n (offset 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t term3 = pow_seq((seq_t)3, n);
    seq_t term8 = pow_seq((seq_t)8, n);
    seq_t term9 = pow_seq((seq_t)9, n);
    return term3 + term8 + term9;
}
