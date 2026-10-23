/*
 * A131996  Number of partitions of n into distinct powers of 2 or of 3.
 * https://oeis.org/A131996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct powers of 2 or 3
 *            (OEIS A131996). The sequence counts the subsets of {1, 2, 4, 8,
 *            …, 3, 9, 27, …} whose sum equals n. Implementation: dynamic
 *            programming over these elements. Works for any integer types
 *            seq_t and idx_t supplied by the build. ---------...
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define MAX_N 20000   /* max n expected (≈10000) */

/* DP array large enough for all calls.  seq_t default constructor
   is expected to produce zero. */
static seq_t dp[MAX_N + 1];

/* Helper: convert a seq_t value known to be ≤ MAX_N to idx_t.
   The precondition (val ≤ MAX_N) is guaranteed by construction. */
static inline idx_t to_idx(seq_t val) {
    return (idx_t)(unsigned long long)val;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for 0 in OEIS offset 1 */

    /* Ensure we are within the reserved range.  For safety we clamp
       the maximum; the implementation is only required for n ≤ 10000. */
    if (n > MAX_N) {
#if defined(DEBUG)
        /* In a debug build we might abort, but here we simply
           return 0 for out‑of‑range. */
#endif
        return 0;
    }

    /* Reset DP table for indices 0..n. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Gather all distinct elements ≤ n. */
    static seq_t elems[64];
    idx_t elem_cnt = 0;

    elems[elem_cnt++] = 1;      /* 2^0 and 3^0 are both 1, but counted once */

    /* Powers of 2: 2,4,8,… */
    seq_t val = 2;
    while (val <= n) {
        elems[elem_cnt++] = val;
        val *= 2;
    }

    /* Powers of 3: 3,9,27,… */
    val = 3;
    while (val <= n) {
        elems[elem_cnt++] = val;
        val *= 3;
    }

    /* DP over collected elements */
    for (idx_t eidx = 0; eidx < elem_cnt; ++eidx) {
        idx_t e = to_idx(elems[eidx]);      /* e ≤ n, fits in idx_t */
        for (idx_t s = n; s >= e; --s) {
            dp[s] += dp[s - e];
        }
    }

    return dp[n];
}
