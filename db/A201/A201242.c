/*
 * A201242  Number of ways to place 8 non-attacking wazirs on an n X n
 *          toroidal board.
 * https://oeis.org/A201242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n^14 - 140*n^12 + 8722*n^10 - 313880*n^8 * +
 *            7061089*n^6 - 99573740*n^4 + 817978188*n^2 * - 3033601200)/40320
 *            for n ≥ 9; * uses special handling for n < 9 by a
 *            transfer‑matrix DP on the * n‑cycle patterns
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: popcount for 64‑bit values */
static inline idx_t popcnt64(uint64_t v) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(v);
#else
    /* Fallback: simple loop */
    idx_t c = 0;
    while (v) { v &= v - 1; ++c; }
    return c;
#endif
}

/* Count ways for n < 9 by DP over cycle patterns */
static seq_t small_n(idx_t n) {
    if (n < 1) return 0;

    /* If there are fewer than 8 cells, no placement possible */
    if (n * n < 8) return 0;

    /* Generate all cyclic patterns without adjacent 1s */
    const idx_t max_mask = (idx_t)1 << n;          /* n ≤ 8 */
    idx_t patterns[256];
    idx_t pop[256];
    idx_t P = 0;
    for (idx_t mask = 0; mask < max_mask; ++mask) {
        int ok = 1;
        for (idx_t i = 0; i < n; ++i) {
            if (mask & ((idx_t)1 << i)) {
                idx_t j = (i + 1) % n;
                if (mask & ((idx_t)1 << j)) { ok = 0; break; }
            }
        }
        if (ok) {
            patterns[P] = mask;
            pop[P] = popcnt64(mask);
            ++P;
        }
    }

    /* DP: dp[cur][mask][cnt] */
    seq_t dp[2][256][9];
    for (int k = 0; k < 2; ++k)
        for (int i = 0; i < P; ++i)
            for (int c = 0; c <= 8; ++c)
                dp[k][i][c] = 0;

    seq_t total = 0;

    /* For each choice of first row pattern */
    for (idx_t first = 0; first < P; ++first) {
        int firstPop = (int)pop[first];
        if (firstPop > 8) continue;          /* impossible to reach 8 */
        int cur = 0, nxt = 1;
        /* Initialize DP for row 0 */
        for (int i = 0; i < P; ++i)
            for (int c = 0; c <= 8; ++c)
                dp[cur][i][c] = 0;
        dp[cur][first][firstPop] = 1;

        /* Process rows 1 .. n-1 */
        for (idx_t row = 1; row < n; ++row) {
            /* Clear next layer */
            for (idx_t i = 0; i < P; ++i)
                for (int c = 0; c <= 8; ++c)
                    dp[nxt][i][c] = 0;

            /* Transition */
            for (idx_t prev = 0; prev < P; ++prev) {
                int popPrev = (int)pop[prev];
                for (int cnt = 0; cnt <= 8; ++cnt) {
                    seq_t ways = dp[cur][prev][cnt];
                    if (!ways) continue;
                    for (idx_t next = 0; next < P; ++next) {
                        if (patterns[prev] & patterns[next]) continue;   /* vertical attack */
                        int newCnt = cnt + (int)pop[next];
                        if (newCnt > 8) continue;
                        dp[nxt][next][newCnt] += ways;
                    }
                }
            }
            /* Swap layers */
            int t = cur; cur = nxt; nxt = t;
        }

        /* After last row, enforce wraparound with first row */
        for (idx_t last = 0; last < P; ++last) {
            if (patterns[last] & patterns[first]) continue;          /* vertical attack across boundary */
            total += dp[cur][last][8];
        }
    }

    return total;
}

/* Main entry function */
seq_t a(idx_t n) {
    /* Use special implementation for n < 9 */
    if (n < 9) return small_n(n);

    /* For n >= 9 use the explicit polynomial formula.
     * Intermediate values are kept in a signed 128‑bit type
     * to avoid overflow for all n that produce a 64‑bit result.
     */
    __int128 N = n;
    __int128 n2 = N * N;             /* n^2   */
    __int128 n4 = n2 * n2;           /* n^4   */
    __int128 n6 = n4 * n2;           /* n^6   */
    __int128 n8 = n4 * n4;           /* n^8   */
    __int128 n10 = n8 * n2;          /* n^10  */
    __int128 n12 = n8 * n4;          /* n^12  */
    __int128 n14 = n8 * n6;          /* n^14  */

    __int128 poly =  n14
                    - (__int128)140 * n12
                    + (__int128)8722 * n10
                    - (__int128)313880 * n8
                    + (__int128)7061089 * n6
                    - (__int128)99573740 * n4
                    + (__int128)817978188 * n2
                    - (__int128)3033601200;

    __int128 num = n2 * poly;
    __int128 denom = 40320;          /* 8! */

    return (seq_t)(num / denom);
}
