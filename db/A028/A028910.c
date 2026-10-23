/*
 * A028910  Arrange digits of 2^n in descending order.
 * https://oeis.org/A028910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = descending_digits(2^n)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   digit_frequencies, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/digit_frequencies.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);
    unsigned int freq[10] = {0};
    digit_frequencies(v, freq);

    seq_t res = 0;
    for (int d = 9; d >= 0; d--)
    {
        unsigned int c = freq[d];
        for (unsigned int i = 0; i < c; i++)
        {
            res = mul_small(res, 10);
            res += d;
        }
    }
    return res;
}
