/*
 * A236239  Number of binary words of length n with exactly 10 (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n.
 * https://oeis.org/A236239
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 62, u64 n <= 84, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of binary words of length n that contain exactly 10
   (possibly overlapping) occurrences of the binary representation of n.
   Computed by dynamic programming over the KMP automaton of the pattern. */

#include <stdbool.h>

#define MAX_K 10
#define MAX_STATE 20   /* maximum pattern length for n ≤ 10000 is 14 */

/* External types supplied by the build: idx_t, seq_t.  The function
   has external linkage as required. */
seq_t a(idx_t n)
{
    /* values below the offset are irrelevant */
    if (n < 15)
        return 0;

    /* ------------------- Build the pattern ------------------- */
    int bits[MAX_STATE];   /* bits of n in reverse order */
    int m = 0;              /* length of the pattern */
    idx_t tmp = n;
    while (tmp > 0) {
        bits[m++] = (int)(tmp & 1ULL);
        tmp >>= 1;
    }
    /* For n > 0, m ≥ 1; no need to handle m==0 */

    /* pattern in normal order (MSB first) */
    int pat[MAX_STATE];
    for (int i = 0; i < m; ++i)
        pat[i] = bits[m - 1 - i];

    /* ------------------- Failure function (pi) ---------------- */
    int pi[MAX_STATE];
    pi[0] = 0;
    for (int i = 1; i < m; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pat[i] != pat[j])
            j = pi[j - 1];
        if (pat[i] == pat[j])
            ++j;
        pi[i] = j;
    }

    /* ------------------- DP over positions -------------------- */
    /* dp[plane][state][k]  state = 0 .. m-1  k = 0 .. 10 */
    seq_t dp[2][MAX_STATE][MAX_K + 1];
    for (int s = 0; s < MAX_STATE; ++s)
        for (int k = 0; k <= MAX_K; ++k)
            dp[0][s][k] = dp[1][s][k] = 0;

    dp[0][0][0] = 1;   /* start with empty string */

    int cur = 0, nxt = 1;
    for (idx_t pos = 0; pos < n; ++pos) {
        /* clear next layer */
        for (int s = 0; s < m; ++s)
            for (int k = 0; k <= MAX_K; ++k)
                dp[nxt][s][k] = 0;

        for (int s = 0; s < m; ++s) {
            for (int k = 0; k <= MAX_K; ++k) {
                seq_t val = dp[cur][s][k];
                if (val == 0)
                    continue;
                for (int bit = 0; bit <= 1; ++bit) {
                    int t = s;
                    while (t > 0 && pat[t] != bit)
                        t = pi[t - 1];
                    if (pat[t] == bit)
                        ++t;
                    int k2 = k + (t == m);
                    if (k2 > MAX_K)
                        continue;
                    if (t == m)
                        t = pi[m - 1]; /* allow overlapping */
                    dp[nxt][t][k2] += val;
                }
            }
        }

        /* swap planes */
        int tmpPlane = cur;
        cur = nxt;
        nxt = tmpPlane;
    }

    /* sum over all final states */
    seq_t result = 0;
    for (int s = 0; s < m; ++s)
        result += dp[cur][s][MAX_K];

    return result;
}
