/*
 * A151782  a(1)=1; for n > 1, a(n)=8*7^{wt(n-1)-1}.
 * https://oeis.org/A151782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 8 * 7^(wt(n-1)-1) where wt(x) is the
 *            number of 1‑bits in the binary representation of x
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Compute the binary weight of n-1 */
    idx_t m = n - 1;
    unsigned int w = 0;
    while (m) {
        w += (unsigned int)(m & 1);
        m >>= 1;
    }

    /* 8 * 7^(w-1) */
    int exp = (int)(w - 1);          /* w >= 1 because n > 1 */
    seq_t result = pow_seq((seq_t)7, exp);
    result *= (seq_t)8;

    return result;
}
