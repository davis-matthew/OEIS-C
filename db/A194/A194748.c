/*
 * A194748  Number of k such that {-k*sqrt(6)} < {-n*sqrt(6)}, where { } =
 *          fractional part.
 * https://oeis.org/A194748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k with 0 ≤ k < n such that
 *            fractional_part(-k*sqrt(6)) < fractional_part(-n*sqrt(6)) (using
 *            the arithmetic test derived from the Beatty sequence of
 *            ⎣k*sqrt(6)⎦)
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 1;  /* k = 0 always satisfies the inequality for n ≥ 1 */
    if (n <= 1)
        return count;

    seq_t n64 = (seq_t)n;
    seq_t fn = floor_sqrt((seq_t)6 * n64 * n64);

    for (idx_t k = 1; k < n; ++k) {
        seq_t k64 = (seq_t)k;
        seq_t fk = floor_sqrt((seq_t)6 * k64 * k64);

        seq_t diff = fn - fk;           /* fn - fk > 0 */
        seq_t nk   = (seq_t)(n - k);    /* (n - k) > 0 */

        seq_t left  = (seq_t)6 * nk * nk;      /* 6*(n-k)^2 */
        seq_t right = diff * diff;             /* (fn - fk)^2 */

        if (left < right)
            ++count;
    }
    return count;
}
