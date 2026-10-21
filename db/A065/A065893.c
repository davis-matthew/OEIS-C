/*
 * A065893  Which composite number is the square of n? Index of n^2 in
 *          A002808.
 * https://oeis.org/A065893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - pi(n^2) - 1
 * Range:     u8 n <= 15, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* compute n^2 as seq_t to avoid overflow of idx_t */
    seq_t sq = (seq_t)n * (seq_t)n;
    /* compute the prime counting function pi(n^2) */
    seq_t pi = compute_pi((idx_t)sq);
    /* composite numbers up to n^2 (excluding 1) */
    return sq - pi - 1;
}
