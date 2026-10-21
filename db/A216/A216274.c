/*
 * A216274  Square array A(n,k) = maximal number of regions into which k-space
 *          can be divided by n hyperplanes (k >= 1, n >= 0), read by
 *          antidiagonals.
 * https://oeis.org/A216274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{i=0..k} C(n-i, i) where (k,n-i) are the anti‑diagonal
 *            * coordinates corresponding to index n. The array A(k,n) defined
 *            by * A(k,n) = Σ_{i=0..k} C(n,i). The sequence a(n) is the
 *            diagonal * reading of this array, with a(0) = 1
 * Range:     u8 n <= 73, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal number d (k+n) that contains index n.
     * The total number of terms up to diagonal d is d*(d+1)/2.
     * Incrementally search for the first d such that d*(d+1)/2 > n.
     * This loop is O(√n) and is very fast for all n used in tests. */
    idx_t d = 0;
    idx_t cum = 0;
    while (cum <= n) {
        ++d;
        cum += d;                 /* cum = d*(d+1)/2 after the addition */
    }

    /* cum now equals d*(d+1)/2 and is > n.
     * The number of elements before this diagonal is cum - d = d*(d-1)/2. */
    idx_t prev_cnt = cum - d;
    idx_t offset = n - prev_cnt;      /* 0 <= offset < d */

    /* Coordinates on this diagonal:
     *   k = d - offset    (k ≥ 1)
     *   m = offset        (m ≥ 0)  where m is the n parameter of A(k,m) */
    idx_t k = d - offset;
    idx_t m = offset;

    /* Compute A(k,m) = Σ_{i=0..k} C(m,i) in a safe way.
     * Use a running binomial coefficient to avoid repeated calls to
     * binomial() and to keep intermediate values small. */
    seq_t sum = 0;
    seq_t comb = 1;                 /* C(m,0) */
    for (idx_t i = 0; i <= k; ++i) {
        if (i > 0) {
            /* comb = C(m,i) = C(m,i-1) * (m-(i-1)) / i */
            comb = comb * (seq_t)(m - (i - 1));
            comb = comb / (seq_t)i;
        }
        sum += comb;
    }

    return sum;
}
