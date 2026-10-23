/*
 * A183879  Number of arrangements of n+2 numbers in 0..4 with each number
 *          being the sum mod 5 of two others.
 * https://oeis.org/A183879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n+2) - (10*n^2 + 70*n + 124)*2^n + 2*(3*n+8)*(n^2 +
 *            5*n + 8) for n >= 2. a(1) = 1. All arithmetic is performed in
 *            the built‑in seq_t type
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* 5^(n+2) and 2^n */
    seq_t pow5 = big_pow((seq_t)5, (seq_t)(n + 2));
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* coefficient (10*n^2 + 70*n + 124) – kept in 64‑bit to avoid overflow */
    uint64_t tmp = 10ULL * (uint64_t)n * (uint64_t)n
                 + 70ULL * (uint64_t)n
                 + 124ULL;
    seq_t coeff = (seq_t)tmp;

    /* part1 = 5^(n+2) - (10*n^2 + 70*n + 124)*2^n */
    seq_t part1 = pow5 - (pow2 * coeff);

    /* part2 = 2*(3*n+8)*(n^2 + 5*n + 8) */
    seq_t term1 = (seq_t)(3 * (idx_t)n + 8);
    seq_t term2 = (seq_t)( (idx_t)n * (idx_t)n + 5 * (idx_t)n + 8 );
    seq_t part2 = (seq_t)2 * term1 * term2;

    return part1 + part2;
}
