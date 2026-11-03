/*
 * A077106  Largest integer cube <= n^2.
 * https://oeis.org/A077106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor_cuberoot((seq_t)n * (seq_t)n)^3 (largest cube <=
 *            n^2)
 * Range:     u8 n <= 15, u16 n <= 261, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9759 (2026-11-01)
 * Helpers:   icbrt_seq, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/icbrt_seq.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;     /* n^2 in seq_t */
    seq_t k     = icbrt_seq(n_sq);   /* k = floor(cuberoot(n^2)) */
    return ipow_i32(k, 3);                 /* k^3 is the desired cube */
}
