/*
 * A202071  Number of arrays of n+2 integers in -3..3 with sum zero and the
 *          sum of every adjacent pair being odd.
 * https://oeis.org/A202071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 195 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A202071
 * Number of arrays of length n+2 of integers in {-3,-2,-1,0,1,2,3}
 * whose total sum is zero and such that the sum of each adjacent pair
 * is odd.  This property forces the array to alternate in parity,
 * so the array consists of "odd" elements (±1,±3) on the odd
 * positions and "even" elements (−2,0,2) on the even positions.
 * The following routine computes the number of such arrays for any
 * n ≥ 1 by dynamic programming for the two parity classes.  The
 * algorithm is O(n²) in the worst case, fast enough for the
 * required limits (n ≈ 10 000).  It uses only the integer operations
 * permitted in the problem statement and works for any seq_t
 * (unsigned integer of arbitrary width).
 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Dynamic programming for the number of ways to obtain each sum
 * using exactly m "even" numbers (−2,0,2). */
static void dp_even(idx_t m, seq_t **out, size_t *len)
{
    size_t sz = 4U * m + 1U;
    seq_t *dp = mem_new(seq_t, sz);
    if (!dp) { *out = NULL; *len = 0; return; }
    size_t offset = 2U * m;
    dp[offset] = 1;                     /* sum 0 */

    for (idx_t step = 1; step <= m; ++step) {
        seq_t *next = mem_new(seq_t, sz);
        if (!next) { mem_free(dp); *out = NULL; *len = 0; return; }
        for (size_t i = 0; i < sz; ++i) {
            seq_t v = dp[i];
            if (!v) continue;
            if (i >= 2U) next[i-2U] += v;   /* +(−2) */
            next[i] += v;                   /* +(0) */
            if (i + 2U < sz) next[i+2U] += v; /* +(2) */
        }
        mem_free(dp);
        dp = next;
    }
    *out = dp;
    *len = sz;                           /* 4*m+1 */
}

/* Dynamic programming for the number of ways to obtain each sum
 * using exactly m "odd" numbers (±1,±3). */
static void dp_odd(idx_t m, seq_t **out, size_t *len)
{
    size_t sz = 6U * m + 1U;
    seq_t *dp = mem_new(seq_t, sz);
    if (!dp) { *out = NULL; *len = 0; return; }
    size_t offset = 3U * m;
    dp[offset] = 1;                     /* sum 0 */

    for (idx_t step = 1; step <= m; ++step) {
        seq_t *next = mem_new(seq_t, sz);
        if (!next) { mem_free(dp); *out = NULL; *len = 0; return; }
        for (size_t i = 0; i < sz; ++i) {
            seq_t v = dp[i];
            if (!v) continue;
            if (i >= 3U) next[i-3U] += v;   /* +(-3) */
            if (i >= 1U) next[i-1U] += v;   /* +(-1) */
            if (i + 1U < sz) next[i+1U] += v; /* +(1) */
            if (i + 3U < sz) next[i+3U] += v; /* +(3) */
        }
        mem_free(dp);
        dp = next;
    }
    *out = dp;
    *len = sz;                           /* 6*m+1 */
}

/* Count arrays for a fixed pattern of m_odd odd positions and m_even
 * even positions.  The total sum must be zero. */
static seq_t count_pattern(idx_t m_odd, idx_t m_even)
{
    seq_t *odd_dp = NULL; size_t sz_o = 0;
    seq_t *even_dp = NULL; size_t sz_e = 0;
    dp_odd(m_odd, &odd_dp, &sz_o);
    if (!odd_dp) return 0;
    dp_even(m_even, &even_dp, &sz_e);
    if (!even_dp) { mem_free(odd_dp); return 0; }

    size_t offset_o = 3U * m_odd;
    size_t offset_e = 2U * m_even;
    seq_t total = 0;

    for (size_t i = 0; i < sz_o; ++i) {
        long long sum_o = (long long)i - (long long)offset_o;
        long long sum_e = -sum_o;
        if (sum_e < -(long long)2 * (long long)m_even ||
            sum_e >  2LL * (long long)m_even)
            continue;
        size_t idx_e = (size_t)((long long)offset_e + sum_e);
        if (idx_e >= sz_e) continue;          /* safety check */
        seq_t cnt_o = odd_dp[i];
        if (!cnt_o) continue;
        seq_t cnt_e = even_dp[idx_e];
        if (!cnt_e) continue;
        total += cnt_o * cnt_e;
    }

    mem_free(odd_dp);
    mem_free(even_dp);
    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Length of the array is n+2 */
    idx_t L = n + 2;
    /* Number of odd‑positioned elements for the two possible patterns */
    idx_t m_odd_start  = (L + 1) / 2;   /* ceil(L/2)   */
    idx_t m_even_start = L / 2;         /* floor(L/2)  */

    seq_t res = 0;
    res += count_pattern(m_odd_start,  m_even_start); /* odd first */
    res += count_pattern(m_even_start, m_odd_start);  /* even first */
    return res;
}
