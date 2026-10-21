/*
 * A058578  McKay-Thompson series of class 24H for Monster.
 * https://oeis.org/A058578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 34, u32 n <= 110, u64 n <= 379,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This implementation computes the OEIS sequence A058578.
 * The sequence is the Euler transform of the period‑12 sequence  
 * [2, 2, 0, 0, 2, 0, 2, 0, 0, 2, 2, 0] and can be expressed as
 * 
 *   a(n) = coefficient of q^n in  
 *          ∏_{k≥1, 3∤k, 4∤k} (1 - q^k)^(-2)
 *
 * The coefficient is obtained by the standard partition DP:
 *   For each allowed part k the factor 1/(1−q^k)^2 equals
 *   ∑_{t≥0}(t+1) q^{t k}.  This can be applied by performing two
 *   successive convolutions with 1/(1−q^k).  Avoiding large
 *   convolutions, we perform a double pass over the array:
 *       cur[j] += cur[j-k]   (first pass)
 *       cur[j] += cur[j-k]   (second pass)
 *   which implements the multiplication by (1−q^k)^(-2) using only
 *   integer additions.
 *
 *   The algorithm runs in O(n²) additions, which is well under the
 *   2‑second time limit for n ≤ 10 000.
 */

#include <stdlib.h>

/* compute a(n) as described above */
seq_t a(idx_t n)
{
    /* size of the coefficient array */
    idx_t N = n;
    size_t size = (size_t)N + 1;

    /* allocate and initialise array */
    seq_t *cur = (seq_t *)malloc(size * sizeof(seq_t));
    if (!cur) return 0;            /* allocation failure – return 0 (unused) */
    for (size_t i = 0; i < size; ++i) cur[i] = 0;
    cur[0] = 1;

    /* iterate over allowed parts (not divisible by 3 or 4) */
    for (idx_t k = 1; k <= N; ++k) {
        if (k % 3 == 0 || k % 4 == 0) continue;
        /* first convolution with 1/(1−q^k)  */
        for (idx_t j = k; j <= N; ++j) cur[j] += cur[j - k];
        /* second convolution: implements the square factor  */
        for (idx_t j = k; j <= N; ++j) cur[j] += cur[j - k];
    }

    seq_t res = cur[N];
    free(cur);
    return res;
}
