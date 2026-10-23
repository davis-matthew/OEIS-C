/*
 * A198643  a(n) = 5*3^n-1.
 * https://oeis.org/A198643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 5*3^n-1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow3_seq(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
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
    seq_t val = pow3_seq(n);
    val = val * (seq_t)5;
    val = val - (seq_t)1;
    return val;
}
