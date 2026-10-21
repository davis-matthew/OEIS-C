/*
 * A236237  Number of binary words of length n with exactly 8 (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n.
 * https://oeis.org/A236237
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 53, u64 n <= 84, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation: number of binary words of length n that contain exactly 8
 * (possibly overlapping) occurrences of the binary pattern corresponding to n.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Number of required matches */
    const idx_t K = 8;
    /* Maximum pattern length for n up to about 10,000 (2^14) */
    const idx_t MAX_M = 64;

    /* Extract binary pattern of n */
    idx_t m = 0;
    idx_t temp = n;
    while (temp > 0) {
        m++;
        temp >>= 1;
    }
    /* In case n is 0, which will never happen for the sequence */
    if (m == 0) m = 1;

    /* Store bits of the pattern (most significant bit first) */
    uint8_t pat[MAX_M];
    for (idx_t i = 0; i < m; i++) {
        pat[i] = (n >> (m - 1 - i)) & 1;
    }

    /* Build prefix function (failure function) for KMP */
    idx_t pi[MAX_M];
    pi[0] = 0;
    for (idx_t i = 1; i < m; i++) {
        idx_t j = pi[i - 1];
        while (j > 0 && pat[i] != pat[j]) j = pi[j - 1];
        if (pat[i] == pat[j]) j++;
        pi[i] = j;
    }

    /* Precompute transitions: next_state[s][b] and match_flag[s][b] */
    idx_t next_state[MAX_M][2];
    bool match_flag[MAX_M][2];
    for (idx_t s = 0; s < m; s++) {
        for (int b = 0; b <= 1; b++) {
            idx_t l = s;
            while (l > 0 && (uint8_t)b != pat[l]) l = pi[l - 1];
            if ((uint8_t)b == pat[l]) l++;
            bool matched = (l == m);
            if (matched) l = pi[m - 1];
            next_state[s][b] = l;
            match_flag[s][b] = matched;
        }
    }

    /* DP arrays: current and next. Dimensions [m][K+1] */
    seq_t dp[MAX_M][K + 1];
    seq_t newdp[MAX_M][K + 1];

    /* Initial state */
    for (idx_t s = 0; s < m; s++)
        for (idx_t c = 0; c <= K; c++)
            dp[s][c] = 0;
    dp[0][0] = 1;

    /* Iterate over positions */
    for (idx_t pos = 0; pos < n; pos++) {
        /* Zero newdp */
        for (idx_t s = 0; s < m; s++)
            for (idx_t c = 0; c <= K; c++)
                newdp[s][c] = 0;

        for (idx_t s = 0; s < m; s++) {
            for (idx_t c = 0; c <= K; c++) {
                seq_t val = dp[s][c];
                if (val == 0) continue;
                for (int b = 0; b <= 1; b++) {
                    idx_t l = next_state[s][b];
                    idx_t nc = c + (match_flag[s][b] ? 1 : 0);
                    if (nc <= K) {
                        newdp[l][nc] += val;
                    }
                }
            }
        }
        /* Copy newdp to dp */
        for (idx_t s = 0; s < m; s++)
            for (idx_t c = 0; c <= K; c++)
                dp[s][c] = newdp[s][c];
    }

    /* Sum over all final states with exactly K matches */
    seq_t result = 0;
    for (idx_t s = 0; s < m; s++) {
        result += dp[s][K];
    }
    return result;
}
