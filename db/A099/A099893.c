/*
 * A099893  XOR BINOMIAL transform of A006068 (inverse Gray code).
 * https://oeis.org/A099893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(0) = 0; a(1) = 1; for n ≥ 2, a(n) = 0 unless n is a power of
 *            two. If n = 2^m (m ≥ 0) then a(n) = 2^(m+1) – 1
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 8076 (2026-11-01)
 * Helpers:   ilog2, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"
#include "../utils/pow2.h"

/* Computes the n-th term of OEIS sequence A099893
 * (XOR BINOMIAL transform of A006068, the inverse Gray code). */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;
    if ((n & (n - 1u)) != 0u) return 0;      /* not a power of two */

    /* n is a power of two: compute m = log₂(n) */
    unsigned m = ilog2(n);
    /* a(n) = 2^(m+1) – 1 */
    return pow2(m + 1) - 1;
}
