/*
 * A219693  The number of symmetric positive definite 2 X 2 matrices whose
 *          entries are integers of absolute value at most n.
 * https://oeis.org/A219693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=1..n} Σ_{b=1..n} (2*min(n, floor_sqrt(a*b-1)) + 1)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, safe return */

    seq_t total = 0;
    for (idx_t a = 1; a <= n; ++a) {
        for (idx_t b = 1; b <= n; ++b) {
            idx_t prod = a * b;          /* prod ≤ n² ≤ 1e8 for n ≤ 10000 */
            idx_t s    = prod - 1;       /* non‑negative */
            idx_t r    = (idx_t)floor_sqrt((seq_t)s);
            if (r > n) r = n;            /* safety, not needed for n ≤ 10000 */
            total += (seq_t)(2 * r + 1);
        }
    }
    return total;
}
