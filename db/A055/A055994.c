/*
 * A055994  Expansion of (1+6x)/(1-x)^10.
 * https://oeis.org/A055994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (7n+9) * C(n+8, 8) / 9
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 35, u64 n <= 457, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= 8; ++i) {
        r = r * (n + i);
        r = r / i;
    }
    r = r * (7 * n + 9);
    r = r / 9;
    return (seq_t)r;
#else
    seq_t coeff = binomial(n + 8, 8);
    coeff = coeff * (seq_t)(7 * n + 9);
    coeff = coeff / 9;
    return coeff;
#endif
}
