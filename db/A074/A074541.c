/*
 * A074541  a(n) = 2^n + 6^n + 7^n.
 * https://oeis.org/A074541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 6^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation for any seq_t type compatible with + and * operators */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p2 = pow_seq_t((seq_t)2, n);
    seq_t p6 = pow_seq_t((seq_t)6, n);
    seq_t p7 = pow_seq_t((seq_t)7, n);
    return p2 + p6 + p7;
}
