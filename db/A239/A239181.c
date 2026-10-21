/*
 * A239181  Number of (3+1)X(n+1) 0..2 arrays with no element greater than all
 *          horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A239181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×(n+1) arrays of digits 0..2 with no element
 *            greater than all horizontal neighbors or equal to all vertical
 *            neighbors
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#define S 36

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t cols = (size_t)(n + 1);   /* number of columns */
    if (cols < 2) return 0;          /* not needed for offset 1, but safe */

    /* ----------  State definitions  ---------- */
    static int state_digits[S][4];
    static bool precede[S][S];
    static bool follow[S][S][S];
    static bool initialized = false;
    if (!initialized) {
        /* Build all 4‑digit states that satisfy the vertical condition:
           v0 ≠ v1  and  v2 ≠ v3. */
        int st = 0;
        for (int id = 0; id < 81; ++id) {   /* 3^4 = 81 possible states */
            int tmp = id;
            int d[4];
            d[0] = tmp % 3; tmp /= 3;
            d[1] = tmp % 3; tmp /= 3;
            d[2] = tmp % 3; tmp /= 3;
            d[3] = tmp % 3;
            if (d[0] != d[1] && d[2] != d[3]) {
                state_digits[st][0] = d[0];
                state_digits[st][1] = d[1];
                state_digits[st][2] = d[2];
                state_digits[st][3] = d[3];
                ++st;
            }
        }
        /* Compute precede[i][j] : state i is ≤ state j in every coordinate. */
        for (size_t i = 0; i < S; ++i) {
            for (size_t j = 0; j < S; ++j) {
                bool ok = true;
                for (int k = 0; k < 4; ++k)
                    if (state_digits[i][k] > state_digits[j][k]) { ok = false; break; }
                precede[i][j] = ok;
            }
        }
        /* Compute follow[a][b][c] : state b is not > both a and c in any coordinate. */
        for (size_t a = 0; a < S; ++a)
            for (size_t b = 0; b < S; ++b)
                for (size_t c = 0; c < S; ++c) {
                    bool ok = true;
                    for (int k = 0; k < 4; ++k)
                        if (state_digits[b][k] > state_digits[a][k] && state_digits[b][k] > state_digits[c][k]) {
                            ok = false;
                            break;
                        }
                    follow[a][b][c] = ok;
                }
        initialized = true;
    }

    /* ----------  DP over columns  ---------- */
    seq_t dp[S][S];
    seq_t dp_next[S][S];
    /* Initialise dp for the first two columns */
    for (size_t i = 0; i < S; ++i)
        for (size_t j = 0; j < S; ++j)
            dp[i][j] = 0;
    for (size_t i = 0; i < S; ++i)
        for (size_t j = 0; j < S; ++j)
            if (precede[i][j])   /* s0 <= s1 (first column ≤ second column) */
                dp[i][j] = 1;

    /* Process columns 3 … cols */
    for (size_t col = 3; col <= cols; ++col) {
        for (size_t i = 0; i < S; ++i)
            for (size_t j = 0; j < S; ++j)
                dp_next[i][j] = 0;
        for (size_t prev = 0; prev < S; ++prev) {
            for (size_t curr = 0; curr < S; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == 0) continue;
                for (size_t nxt = 0; nxt < S; ++nxt) {
                    if (follow[prev][curr][nxt])          /* curr not > both prev and nxt */
                        dp_next[curr][nxt] += val;
                }
            }
        }
        /* copy dp_next into dp for next iteration */
        for (size_t i = 0; i < S; ++i)
            for (size_t j = 0; j < S; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* ----------  Final column constraint  ---------- */
    seq_t result = 0;
    for (size_t prev = 0; prev < S; ++prev)
        for (size_t curr = 0; curr < S; ++curr)
            if (precede[curr][prev])                  /* last column ≤ second‑last column */
                result += dp[prev][curr];

    return result;
}
