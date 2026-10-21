/*
 * A236238  Number of binary words of length n with exactly 9 (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n.
 * https://oeis.org/A236238
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 53, u64 n <= 84, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of binary words of length n with exactly 9 (possibly overlapping)
 * occurrences of the subword equal to the binary expansion of n.
 * Computed by building the KMP automaton for the pattern,
 * then dynamic programming over alphabet {0,1}.
 */
#include <stddef.h>
#include <string.h>  /* for memset */

#define MAX_STATES 16  /* maximum pattern length + 1 (n < 16384 => len <= 14) */
#define MAX_K      10  /* we need exactly 9 matches */

/* Helper to zero a 2‑D array of seq_t */
static void zero_array(seq_t arr[MAX_STATES][MAX_K]) {
    for (int s = 0; s < MAX_STATES; ++s)
        for (int k = 0; k < MAX_K; ++k)
            arr[s][k] = 0;
}

seq_t a(idx_t n) {
    const idx_t K = 9;
    if (n < 15) /* undefined behaviour below offset, return 0 */
        return 0;

    /* ---------- 1. Build binary pattern of n (MSB first) ---------- */
    int bits_rev[64];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        bits_rev[len++] = (int)(tmp & 1);
        tmp >>= 1;
    }
    if (len == 0) /* should not happen for n>=15 */
        return 0;

    int pat[MAX_STATES];
    for (int i = 0; i < len; ++i)
        pat[i] = bits_rev[len - 1 - i];  /* MSB first */

    /* ---------- 2. Failure function (prefix function) ---------- */
    int fail[MAX_STATES];
    fail[0] = 0;
    for (int i = 1; i < len; ++i) {
        int j = fail[i - 1];
        while (j > 0 && pat[i] != pat[j])
            j = fail[j - 1];
        if (pat[i] == pat[j])
            ++j;
        fail[i] = j;
    }

    /* ---------- 3. Transition table next_state[state][bit] ---------- */
    int next_state[MAX_STATES][2];
    for (int s = 0; s < len; ++s) {
        for (int bit = 0; bit < 2; ++bit) {
            int t = s;
            while (t > 0 && pat[t] != bit)
                t = fail[t - 1];
            if (pat[t] == bit)
                ++t;
            next_state[s][bit] = t;  /* t in [0..len] */
        }
    }
    /* Transitions from the full‑match state */
    for (int bit = 0; bit < 2; ++bit) {
        int t = fail[len - 1];
        while (t > 0 && pat[t] != bit)
            t = fail[t - 1];
        if (pat[t] == bit)
            ++t;
        next_state[len][bit] = t;
    }

    /* ---------- 4. DP over positions ---------- */
    seq_t cur[MAX_STATES][MAX_K];
    seq_t next_arr[MAX_STATES][MAX_K];
    zero_array(cur);
    zero_array(next_arr);

    cur[0][0] = 1;  /* start at state 0 with 0 matches */

    for (idx_t pos = 0; pos < n; ++pos) {
        zero_array(next_arr);
        for (int s = 0; s <= len; ++s) {
            for (int k = 0; k <= K; ++k) {
                seq_t val = cur[s][k];
                if (val == 0) continue;
                for (int bit = 0; bit < 2; ++bit) {
                    int t = next_state[s][bit];
                    int nk = k + (t == len ? 1 : 0);
                    if (nk <= K)
                        next_arr[t][nk] += val;
                }
            }
        }
        /* copy next_arr back to cur for next iteration */
        for (int s = 0; s <= len; ++s)
            for (int k = 0; k <= K; ++k)
                cur[s][k] = next_arr[s][k];
    }

    /* ---------- 5. Sum over all final states with exactly K matches ---------- */
    seq_t result = 0;
    for (int s = 0; s <= len; ++s)
        result += cur[s][K];

    return result;
}
