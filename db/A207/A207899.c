/*
 * A207899  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays avoiding 0 0 0 and 1 0 1
 *            horizontally and 0 0 0 and 1 1 1 vertically
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* the function to compute the sequence */
seq_t a(idx_t n)
{
    /* Early return for the offset */
    if (n == 0) return (seq_t)0;          /* value before offset is irrelevant */

    /* static storage for the precomputation */
    static uint8_t col_bits[26];          /* 6‑bit pattern of each valid column */
    static bool triple_ok[26][26][26];    /* horizontal compatibility of triples */
    static bool ready = false;

    /* Build the list of valid columns (no 000 and no 111 within a column) */
    if (!ready) {
        int count = 0;
        for (uint32_t mask = 0; mask < (1u << 6); ++mask) {
            bool ok = true;
            for (int r = 0; r <= 3; ++r) {
                int a = (mask >> r) & 1;
                int b = (mask >> (r + 1)) & 1;
                int c = (mask >> (r + 2)) & 1;
                if (a == b && b == c) {  /* 000 or 111 inside the column */
                    ok = false;
                    break;
                }
            }
            if (ok) {
                col_bits[count++] = (uint8_t)mask;
            }
        }
        /* count should be 26 for height 6 */
        /* precompute validity of triples for horizontal patterns */
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                for (int k = 0; k < 26; ++k) {
                    bool good = true;
                    for (int r = 0; r < 6; ++r) {
                        int a = (col_bits[i] >> r) & 1;
                        int b = (col_bits[j] >> r) & 1;
                        int c = (col_bits[k] >> r) & 1;
                        if (a == 0 && b == 0 && c == 0) {          /* horizontal 000 */
                            good = false;
                            break;
                        }
                        if (a == 1 && b == 0 && c == 1) {          /* horizontal 101 */
                            good = false;
                            break;
                        }
                    }
                    triple_ok[i][j][k] = good;
                }
            }
        }
        ready = true;
    }

    /* Handle first two terms directly */
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)676; /* 26^2 */

    /* DP arrays: dp[i][j] = number of ways ending with column pair (i, j) */
    static seq_t dp[26][26];
    static seq_t newdp[26][26];

    /* Initialize for n==2 */
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            dp[i][j] = (seq_t)1;
        }
    }

    /* Extend the sequence from length 3 up to n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero newdp */
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                newdp[i][j] = (seq_t)0;
            }
        }
        /* transition */
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                seq_t val = dp[i][j];
                if (val == (seq_t)0) continue;
                for (int k = 0; k < 26; ++k) {
                    if (triple_ok[i][j][k]) {
                        newdp[j][k] += val;
                    }
                }
            }
        }
        /* move newdp to dp */
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                dp[i][j] = newdp[i][j];
            }
        }
    }

    /* sum all states for the final answer */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            ans += dp[i][j];
        }
    }
    return ans;
}
