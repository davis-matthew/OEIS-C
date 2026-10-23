/*
 * A175608  Characteristic function of squarefree triangular integers: 1 if
 *          n(n+1)/2 is squarefree else 0.
 * https://oeis.org/A175608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008966(n*(n+1)/2)
 * Range:     u8 n <= 10, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute the n‑th triangular number. */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* If any prime square divides t, the number is not square‑free. */
    seq_t limit = floor_sqrt(t);

    for (idx_t i = 2; (seq_t)i <= limit; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;
        if (t % sq == 0)
            return 0;
    }

    return 1;
}
