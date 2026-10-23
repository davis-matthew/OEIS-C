/*
 * A109415  a(n) equals the (n*(n+1)/2)-th partial sum of the self-convolution
 *          4th power of A010054, which has the g.f.: Sum_{k>=0}
 *          x^(k*(k+1)/2).
 * https://oeis.org/A109415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n*(n+1)/2)] [Sum_{k>=0} x^(k*(k+1)/2)]^4/(1-x)
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external linkage required by the framework */
seq_t a(idx_t n)
{
    /* Triangular number N = n*(n+1)/2 */
    idx_t N = (n * (n + 1)) / 2;
    idx_t K = n + 1;                     /* number of triangular indices 0..n */

    /* triangular numbers T[i] */
    idx_t *tri = (idx_t *)malloc((size_t)K * sizeof(idx_t));
    if (!tri) return 0;                  /* memory failure, should not happen */

    for (idx_t i = 0; i < K; ++i) {
        tri[i] = i * (i + 1) / 2;
    }

    /* pair_counts array (will become prefix sums) */
    uint32_t *pref = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    if (!pref) {
        free(tri);
        return 0;
    }

    /* build counts of unordered pairs (ordered pairs are counted)   */
    /* pair_counts[s] = #{(a,b) | T[a]+T[b] = s}   */
    for (idx_t i = 0; i < K; ++i) {
        idx_t Ti = tri[i];
        for (idx_t j = 0; j < K; ++j) {
            idx_t s = Ti + tri[j];
            if (s > N) break;            /* further j give larger sums */
            pref[s] += 1;
        }
    }

    /* transform to prefix sums: pref[s] = #pairs with sum <= s */
    for (idx_t i = 1; i <= N; ++i) {
        pref[i] += pref[i - 1];
    }

    /* sum over all ordered triples (a,b,c) and add all possible d
       such that T[a]+T[b]+T[c]+T[d] <= N
       The count of d for a given rem = N - T[a] - T[b] - T[c]
       equals pref[rem] (pairs (c,d) with sum <= rem)          */
    unsigned long long result = 0;
    for (idx_t i = 0; i < K; ++i) {
        idx_t Ti = tri[i];
        for (idx_t j = 0; j < K; ++j) {
            idx_t sum_ab = Ti + tri[j];
            if (sum_ab > N) break;
            idx_t rem = N - sum_ab;
            result += pref[rem];
        }
    }

    free(tri);
    free(pref);
    return (seq_t)result;
}
