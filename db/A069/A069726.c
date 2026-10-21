/*
 * A069726  Number of rooted planar bi-Eulerian maps with 2n edges.
 *          Bi-Eulerian: all its vertices and faces are of even valency.
 * https://oeis.org/A069726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 3^(n-1) * binomial(3n, n+1) / (n * (2n+1)), with a(0) =
 *            1. The function uses the helper pow3 from the utilities to
 *            compute powers of 3. The loop computes the binomial coefficient
 *            multiplicatively, guaranteeing exact integer division at each
 *            step
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute binomial(3n, n+1) */
    seq_t N = (seq_t)3 * (seq_t)n;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n + 1; ++i) {
        seq_t term = N - (seq_t)i + 1;
        res = (res * term) / (seq_t)i;  /* exact division */
    }

    /* Divide by n*(2n+1) to obtain A000139(n) */
    seq_t denom = (seq_t)n * (seq_t)(2 * n + 1);
    res = res / denom;

    /* Multiply by 3^(n-1) */
    res = res * pow3(n - 1);

    return res;
}
