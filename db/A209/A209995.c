/*
 * A209995  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          determinant in {0,1,...,n}.
 * https://oeis.org/A209995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×2 matrices with entries in {0,…,n} and
 *            determinant in {0,…,n}
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate array for product counts of size n²+1 */
    size_t N = (size_t)n * n + 1;
    uint32_t *freq = (uint32_t *)calloc(N, sizeof(uint32_t));
    if (!freq) return 0;

    /* Count pairs (b, c) with product = 0 */
    freq[0] = 2u * (uint32_t)n + 1u;   /* (0,0), (0,1..n), (1..n,0) */

    /* Count pairs with positive product */
    for (idx_t b = 1; b <= n; ++b) {
        for (idx_t c = 1; c <= n; ++c) {
            ++freq[(size_t)(b * c)];
        }
    }

    /* Convert freq into prefix sums: freq[i] = #pairs with product ≤ i */
    for (size_t i = 1; i < N; ++i) {
        freq[i] += freq[i - 1];
    }

    /* Now sum over all (a,d) pairs */
    seq_t ans = 0;
    for (idx_t a = 0; a <= n; ++a) {
        idx_t X = 0;                         /* a*d for current d */
        for (idx_t d = 0; d <= n; ++d) {
            size_t idx = (size_t)X;
            uint32_t diff = freq[idx];
            if (X > n) diff -= freq[(size_t)(X - n - 1)];
            ans += (seq_t)diff;
            X += a;                          /* next product: a*(d+1) */
        }
    }

    free(freq);
    return ans;
}
