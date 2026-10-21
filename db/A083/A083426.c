/*
 * A083426  a(n) = (4*7^n+2^n)/5.
 * https://oeis.org/A083426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*7^n + 2^n)/5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t seq_pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow7 = seq_pow_seq_t((seq_t)7, n);
    seq_t pow2 = seq_pow_seq_t((seq_t)2, n);
    seq_t res  = ((seq_t)4 * pow7 + pow2) / (seq_t)5;
    return res;
}
