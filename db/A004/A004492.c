/*
 * A004492  Tersum n + 3.
 * https://oeis.org/A004492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = tersum(n, 3), where tersum adds in base 3 with no carries
 *            (digit-wise mod 3 addition)
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tersum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/tersum.h"

seq_t a(idx_t n) {
    return tersum((seq_t)n, 3);
}
