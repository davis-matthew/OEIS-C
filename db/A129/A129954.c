/*
 * A129954  Second differences of A129952.
 * https://oeis.org/A129954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3; for n > 1, a(n) = (n+4) * 2^(n-2)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)3;
    }
    seq_t factor = (seq_t)(n + 4);
    seq_t pw = pow2(n - 2);
    return factor * pw;
}
