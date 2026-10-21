/*
 * A067057  Let A(n) = {1,2,3,...n}. Let B(r) and C(n-r) be two subsets of
 *          A(n) having r and n-r elements respectively, such that B(r) U
 *          C(n-r) = A(n) and B and C are disjoint; then a(n) = sum of the
 *          products of all combination sums of elements of B and C for r =1
 *          to n-1.
 * https://oeis.org/A067057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-2) * s(n+1, n-1) (s = unsigned Stirling numbers of
 *            the first kind)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t cache_size = 0;
static idx_t computed_max = 0;
static idx_t prev_N = 0;
static seq_t prev_s_nm2 = 0;
static seq_t pow2_val = 0;

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* a(1) = 0 by definition; for n=0 (below offset) return 0 for safety */
        return 0;
    }

    /* Expand cache if needed */
    if ((size_t)(n + 1) > cache_size) {
        a_cache = (seq_t*)realloc(a_cache,
            (size_t)(n + 1) * sizeof(seq_t));
        cache_size = n + 1;
    }

    /* Lazy initialization: compute base values up to n = 2 */
    if (computed_max == 0) {
        a_cache[1] = 0;   /* a(1) */
        a_cache[2] = 2;   /* a(2) = 2^(0) * s(3,1) = 2 */
        computed_max = 2;

        /* s(3,1) for next step */
        prev_N    = 3;          /* current N in recurrence (n+1) */
        prev_s_nm2 = (seq_t)2;  /* s(3,1) */

        /* power of two for exponent (n-2), currently 0 for n=2 */
        pow2_val = 1;  /* 2^0 */
    }

    /* If requested value already computed, return it */
    if (n <= computed_max) {
        return a_cache[n];
    }

    /* Compute a(n) and intermediate values up to requested n */
    idx_t target_N = n + 1;  /* we need s(target_N, target_N-2) */

    for (idx_t curN = prev_N + 1; curN <= target_N; ++curN) {
        /* combinatorial term C(curN-1, 2) */
        idx_t m = curN - 1;
        seq_t comb2 = ((seq_t)m * (m - 1)) / 2;

        /* recurrence: s(curN, curN-2) = s(prev_N, prev_N-2) + (curN-1) * C(curN-1, 2) */
        seq_t s_cur = prev_s_nm2 + ((seq_t)(curN - 1) * comb2);

        /* advance the power of two: exponent increases by 1 each step */
        pow2_val = pow2_val * 2;

        /* a_n = 2^(n-2) * s(n+1, n-1) */
        seq_t a_val = s_cur * pow2_val;

        /* Store result: index in cache is curN - 1 = n */
        a_cache[curN - 1] = a_val;

        /* Update for next iteration */
        prev_N = curN;
        prev_s_nm2 = s_cur;
        computed_max = curN - 1;
    }

    return a_cache[n];
}
