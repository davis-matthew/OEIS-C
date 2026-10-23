/*
 * A100400  Triangle read by rows: T(n,k) is the number of nonroot nodes of
 *          outdegree k (0<=k<=n-1) in all non-crossing trees with n edges.
 * https://oeis.org/A100400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k+1) * binomial(3r - k - 2, 2r - 1) where r is the row
 *            containing the nth element of the triangle read by rows (r = 1
 *            has 1 element, r = 2 has 2, …) and k = n - (1 + 2 + … + (r-1))
 *            (0‑based column)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial helper is provided by the build system: */
#include "../utils/binom_big.h"

seq_t a(idx_t n)
{
    /* Offset: the first valid term is a(1) = 1 */
    if (n < 1)
        return 0;

    /* Find the row r containing the nth element.
       We accumulate the running sum of row sizes until we surpass n. */
    idx_t r = 1;
    idx_t total = 1;                 /* Total elements up to current row */
    while (total < n) {
        r++;
        total += r;
    }

    /* Index of the first element in row r */
    idx_t start = total - r + 1;
    /* 0‑based column within the row */
    idx_t k = n - start;

    /* Compute T(r,k) = (k+1) * binomial(3r - k - 2, 2r - 1) */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    seq_t m  = 3 * sr - sk - (seq_t)2;          /* 3r - k - 2 */
    seq_t kbin = 2 * sr - (seq_t)1;              /* 2r - 1 */

    seq_t bin = binom_big(m, kbin);
    seq_t result = (sk + (seq_t)1) * bin;
    return result;
}
