/*
 * A199747  a(n) = binomial(n*(3*n+1)/2, n).
 * https://oeis.org/A199747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n*(3*n+1)/2, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 300 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    // Compute k = n * (3*n + 1) / 2  (as a idx_t)
    idx_t k = n * (((idx_t)3 * n) + 1) / 2;
    // Return the binomial coefficient C(k, n)
    return binomial(k, n);
}
