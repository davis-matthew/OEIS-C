/*
 * A239443  a(n) = phi(n^9), where phi = A000010.
 * https://oeis.org/A239443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^8 * phi(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   _phi_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_phi_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t ns = (seq_t)n;
    seq_t phi_n = _phi_uint64(n);

    seq_t pow_n8 = (seq_t)1;
    for (int i = 0; i < 8; ++i)
        pow_n8 *= ns;

    return pow_n8 * phi_n;
}
