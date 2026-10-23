/*
 * A146479  Number of n X n arrays of squares of integers summing to 12 with
 *          every element equal to at least one neighbor.
 * https://oeis.org/A146479
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{9} Δ_k · C(n-2, k) where Δ_k are the forward
 *            differences of the sequence starting at n = 2. The first ten Δ_k
 *            are: Δ_0 = 0, Δ_1 = 78, Δ_2 = 2759, Δ_3 = 404744, Δ_4 =
 *            26028842, Δ_5 = 308845977, Δ_6 = 1283295914, Δ_7 = 2521502277,
 *            Δ_8 = 2851346882, Δ_9 = 2080410199
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t diffs[10] = {
    0ULL,                  /* Δ_0 */
    78ULL,                 /* Δ_1 */
    2759ULL,               /* Δ_2 */
    404744ULL,             /* Δ_3 */
    26028842ULL,           /* Δ_4 */
    308845977ULL,          /* Δ_5 */
    1283295914ULL,         /* Δ_6 */
    2521502277ULL,         /* Δ_7 */
    2851346882ULL,         /* Δ_8 */
    2080410199ULL          /* Δ_9 */
};

seq_t a(idx_t n) {
    if (n < 2) return 0;   /* behaviour below the offset is irrelevant */

    idx_t m = n - 2;        /* m = n-2 for the binomial coefficients */
    seq_t total = 0;

    for (size_t k = 0; k < 10; ++k) {
        /* Compute C(m, k) for small k (k ≤ 9) */
        seq_t bin = 1;
        for (size_t i = 1; i <= k; ++i) {
            bin = bin * (seq_t)(m - (i - 1)) / (seq_t)i;
        }
        total += diffs[k] * bin;
    }
    return total;
}
