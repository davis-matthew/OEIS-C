/*
 * A050508  Golden rectangle numbers: n * A007067(n).
 * https://oeis.org/A050508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n * round(n * φ) where φ = (1 + √5)/2 The nearest integer
 *            to n * φ can be computed as round(n*φ) = ⌊(n + n*√5 + 1)/2⌋.
 *            Using a high‑precision integer approximation of √5 we avoid
 *            floating‑point arithmetic
 * Range:     u8 n <= 12, u16 n <= 201, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t PHI_P = 1000000000000ULL;     /* 10^12  */
static const uint64_t SQRT5_MULT = 2236067977499ULL;/* floor(√5 * 10^12) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute nearest integer to n * φ using integer arithmetic */
    uint64_t num = (uint64_t)n * (PHI_P + SQRT5_MULT) + PHI_P;
    uint64_t nearest = num / (2 * PHI_P);

    /* Result: n * nearest, using seq_t operations */
    seq_t n_seq = (seq_t)n;
    seq_t nearest_seq = (seq_t)nearest;

    return n_seq * nearest_seq;
}
