/*
 * A105193  Number of divisors of n1001.
 * https://oeis.org/A105193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of (10000*n + 1001)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 81 (2026-11-01)
 * Helpers:   num_divisors_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors_idx.h"

seq_t a(idx_t n)
{
    idx_t N = n * 10000ULL + 1001ULL;
    return num_divisors_idx(N);
}
