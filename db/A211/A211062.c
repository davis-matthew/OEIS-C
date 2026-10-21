/*
 * A211062  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant >= 2n.
 * https://oeis.org/A211062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 306, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of 2×2 matrices with entries in {1,…,n} 
 * whose determinant ≥ 2n.  The computation uses
 *   cnt[p] = #{(a,d) : 1≤a,d≤n , a·d = p}
 *   f(v)   = #{(b,c) : 1≤b,c≤n , b·c ≤ v}
 * Then  
 *   a(n) = ∑_{p=1}^{n²} cnt[p] · f(p‑2n)
 * where f(v)=0 for v≤0.  The implementation builds cnt[p] by
 * a double loop and evaluates the sum in linear time with a
 * sliding‑window technique that keeps only O(n) auxiliary memory.
 */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* Size of product range */
    idx_t N = n * n;                /* 1 ≤ p ≤ n²  */

    /* Allocate count array for each product */
    uint16_t *cnt = (uint16_t *)calloc((size_t)N + 1, sizeof(uint16_t));
    if (!cnt) return (seq_t)0;      /* allocation failed */

    /* Count pairs (a,d) with product p */
    for (idx_t a = 1; a <= n; ++a) {
        for (idx_t d = 1; d <= n; ++d) {
            ++cnt[a * d];
        }
    }

    /* Sliding‑window for f(p‑2n) = sum cnt[1..p‑2n] */
    size_t buf_size = 2 * (size_t)n - 1;          /* 2n‑1 entries */
    uint16_t *buf = (uint16_t *)calloc(buf_size, sizeof(uint16_t));
    if (!buf) {
        free(cnt);
        return (seq_t)0;
    }
    size_t buf_idx = 0;
    seq_t sum_win = 0;   /* sum of the last 2n‑1 counts */
    seq_t cumulative = 0;/* cumulative sum of counts up to current p-1 */
    seq_t ans = 0;

    for (idx_t p = 1; p <= N; ++p) {
        uint16_t cp = cnt[p];
        /* pref = sum_{t=1}^{p-2n} cnt[t]  = cumulative - sum_win */
        seq_t pref = cumulative - sum_win;
        ans += (unsigned __int128)cp * pref;

        /* Update running totals */
        cumulative += cp;

        /* Update sliding window */
        sum_win -= buf[buf_idx];
        buf[buf_idx] = cp;
        sum_win += cp;
        buf_idx = (buf_idx + 1) % buf_size;
    }

    free(cnt);
    free(buf);
    return ans;
}
