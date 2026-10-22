/*
 * A004945  a(n) = round(n*phi^10), where phi is the golden ratio, A001622.
 * https://oeis.org/A004945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*(123+55*sqrt(5))/2) = floor((123*n + 1 +
 *            floor(sqrt(15125*n^2)))/2)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 1305, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004945_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a004945_isqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t m = a004945_isqrt(nn * nn * 15125);
    return (nn * 123 + 1 + m) / 2;
}
