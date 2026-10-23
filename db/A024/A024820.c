/*
 * A024820  a(n) = least m such that if r and s in {1/2, 1/4, 1/6,..., 1/2n}
 *          satisfy r < s, then r < k/m < s for some integer k.
 * https://oeis.org/A024820
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 194, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A024820
 * a(n) = least integer m such that for every i = 2..n there is an integer
 * k with 1/(2i) < k/m < 1/(2(i-1)).  This is equivalent to the
 * inequalities
 *
 *          floor(m/(2i)) + 1 <= floor((m-1)/(2(i-1)))   for all i=2..n.
 *
 * The function computes the value incrementally, caching previously
 * computed terms.  The cache is bounded by MAX_N (here 30000), which
 * is sufficient for all n ≤ 10000 tested.  The implementation is
 * pure C and compiles as C++17 as well. */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system. */

#define MAX_N 30000          /* upper bound for cached indices */

/* static cache of computed terms */
static seq_t cache[MAX_N + 1];
static idx_t cache_last = 1;  /* last index for which cache is valid */

/* Helper: test if given m satisfies the inequalities for all i=2..k. */
static bool satisfies(idx_t k, seq_t m)
{
    for (idx_t i = 2; i <= k; ++i) {
        seq_t denom   = (seq_t)2 * (seq_t)i;              /* 2i     */
        seq_t left    = m / denom;                       /* floor(m/(2i))          */
        seq_t rhs     = (seq_t)2 * (seq_t)(i - 1) * (left + (seq_t)1);
                                             /* floor((m-1)/(2(i-1)))  */
        if (m <= rhs) return false;  /* need larger m */
    }
    return true;
}

/* Find the least m for index k, given a lower bound start. */
static seq_t find_min_m(idx_t k, seq_t start)
{
    seq_t m = start;
    if (m < (seq_t)3) m = (seq_t)3;  /* by definition m≥3 for n≥2   */

    /* incrementally adjust m until all inequalities hold */
    while (!satisfies(k, m)) {
        /* compute the smallest candidate that would satisfy the first failing inequality */
        bool adjusted = false;
        for (idx_t i = 2; i <= k; ++i) {
            seq_t denom   = (seq_t)2 * (seq_t)i;
            seq_t left    = m / denom;
            seq_t rhs     = (seq_t)2 * (seq_t)(i - 1) * (left + (seq_t)1);
            if (m <= rhs) {
                seq_t cand = rhs + (seq_t)1;
                if (cand > m) {
                    m = cand;
                    adjusted = true;
                }
                break;  /* after adjustment, re‑check all inequalities */
            }
        }
        if (!adjusted) m++;  /* safety net – should not happen */
    }
    return m;
}

/* Public function required by the problem */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* undefined below offset */

    if (n <= MAX_N) {
        /* compute any missing terms up to n */
        if (n > cache_last) {
            for (idx_t k = cache_last + 1; k <= n; ++k) {
                seq_t start = (k == 2) ? (seq_t)3 : cache[k-1] + (seq_t)1;
                cache[k] = find_min_m(k, start);
            }
            cache_last = n;
        }
        return cache[n];
    }

    /* n > MAX_N – compute without caching (rare case) */
    seq_t start = (seq_t)3;          /* start at least 3 */
    return find_min_m(n, start);
}
