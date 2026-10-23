/*
 * A010963  Binomial coefficient C(47,n).
 * https://oeis.org/A010963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(47, n) (returns 0 for n > 47)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n > 47) {
        return 0;
    }
    return binomial((idx_t)47, n);
}
