/*
 * A156183  Denominator of Euler(n, 1/5).
 * https://oeis.org/A156183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 5^n * (1 if n even else 2^{v2(n+1)})
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t res = seq_pow((seq_t)5, n);
    if (n & 1) {
        idx_t v = n + 1;
        unsigned int v2 = 0;
        while ((v & 1) == 0) {
            v2++;
            v >>= 1;
        }
        seq_t factor = seq_pow((seq_t)2, v2);
        res *= factor;
    }
    return res;
}
