/*
 * A207309  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays avoiding 0 0 0 and 1 0 1
 *            horizontally and 0 0 1 and 1 1 0 vertically (recurrence not
 *            shown)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* auxiliary: test whether a 6‑bit column satisfies vertical constraints */
static inline bool valid_column(idx_t col)
{
    for (idx_t i = 0; i + 2 < 6; ++i) {
        int b0 = (int)((col >> i) & 1U);
        int b1 = (int)((col >> (i + 1)) & 1U);
        int b2 = (int)((col >> (i + 2)) & 1U);
        if ((b0 == 0 && b1 == 0 && b2 == 1) ||
            (b0 == 1 && b1 == 1 && b2 == 0))
            return false;
    }
    return true;
}

/* auxiliary: test whether a triple of columns satisfies horizontal constraints */
static inline bool allowed_transition(idx_t a, idx_t b, idx_t c)
{
    for (idx_t r = 0; r < 6; ++r) {
        int ar = (int)((a >> r) & 1U);
        int br = (int)((b >> r) & 1U);
        int cr = (int)((c >> r) & 1U);
        int pat = (ar << 2) | (br << 1) | cr;
        if (pat == 0 || pat == 5)   /* 000 or 101 forbidden */
            return false;
    }
    return true;
}

/* maximum number of valid columns is ≤64, so 64×64 fits in 4096 */
#define MAX_SZ 4096

seq_t a(idx_t n)
{
    /* build list of valid columns (once) */
    static bool init = false;
    static idx_t cols[64];
    static idx_t M = 0;

    if (!init) {
        M = 0;
        for (idx_t col = 0; col < 64; ++col)
            if (valid_column(col))
                cols[M++] = col;
        init = true;
    }

    if (n == 1) return (seq_t)M;        /* one column */
    if (n == 2) return (seq_t)M * (seq_t)M;  /* two independent columns */

    /* dynamic programming tables */
    static seq_t dp[MAX_SZ];
    static seq_t next[MAX_SZ];
    size_t sz = (size_t)M * (size_t)M;

    /* initialise for length 2: all pairs are allowed */
    for (size_t i = 0; i < sz; ++i)
        dp[i] = (seq_t)1;

    /* iterate from length 3 up to n */
    for (idx_t len = 3; len <= n; ++len) {
        for (size_t i = 0; i < sz; ++i)
            next[i] = (seq_t)0;

        for (idx_t a_idx = 0; a_idx < M; ++a_idx) {
            idx_t ca = cols[a_idx];
            size_t abase = (size_t)a_idx * M;
            for (idx_t b_idx = 0; b_idx < M; ++b_idx) {
                seq_t cur = dp[abase + b_idx];
                if (!cur) continue;
                idx_t cb = cols[b_idx];
                for (idx_t c_idx = 0; c_idx < M; ++c_idx) {
                    if (!allowed_transition(ca, cb, cols[c_idx]))
                        continue;
                    next[(size_t)b_idx * M + c_idx] += cur;
                }
            }
        }

        /* swap dp and next */
        for (size_t i = 0; i < sz; ++i)
            dp[i] = next[i];
    }

    /* sum all entries for total number of sequences of length n */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < sz; ++i)
        total += dp[i];

    return total;
}
