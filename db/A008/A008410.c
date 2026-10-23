/*
 * A008410  a(0) = 1, a(n) = 480*sigma_7(n).
 * https://oeis.org/A008410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 480 * sigma_7(n), a(0) = 1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 9, u64 n <= 233, u128 n <= 10000,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t s = sigma_k(n, 7);
    return (seq_t)480 * s;
}
