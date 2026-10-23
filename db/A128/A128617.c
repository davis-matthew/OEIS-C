/*
 * A128617  Expansion of q^2 * psi(q) * psi(q^15) in powers of q where psi()
 *          is a Ramanujan theta function.
 * https://oeis.org/A128617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A128617:  a(n) = #{ (i,j)≥0 : i(i+1)/2 + 15 j(j+1)/2 = n-2 }
    (Number of representations of n-2 by the sum of a triangular number
    and 15 times a triangular number.)
    The computation uses a direct double‑loop over i and a triangularity test
    for the remaining value. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline bool is_triangular(seq_t m)
{
    /* m is triangular iff 8*m+1 is a perfect square. */
    seq_t s = 8 * m + 1;
    seq_t r = floor_sqrt(s);
    return r * r == s;
}

seq_t a(idx_t n)
{
    /* Offset is 1, so a(1)=0. For n<2 there is no representation. */
    if (n < 2) return 0;

    idx_t target_idx = n - 2;          /* value to represent */
    seq_t target = (seq_t)target_idx;
    seq_t count = 0;

    for (idx_t i = 0; ; ++i)
    {
        seq_t tri = (seq_t)i * (i + 1) / 2;
        if (tri > target) break;

        seq_t left = target - tri;
        if (left % 15 != 0) continue;

        seq_t m = left / 15;
        if (is_triangular(m)) ++count;
    }

    return count;
}
