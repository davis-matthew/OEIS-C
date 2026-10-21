/*
 * A071180  Number of numbers m with n < m <= n^2 containing n in decimal
 *          representation.
 * https://oeis.org/A071180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{m | n < m ≤ n² and decimal representation of m contains
 *            that of n}|
 * Range:     u8 n <= 15, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   num_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a digit‑DP that counts how many integers
   ≤ bound contain a given decimal pattern.  For each n we compute
   the count for bound = n² and subtract the count for bound = n;
   the difference yields the desired value a(n). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_to_str.h"

/* Forward declarations – seq_t and idx_t are supplied by the build. */
static seq_t count_leq(seq_t bound, const char *pat, int pat_len);

/* Digit DP that counts integers ≤ bound which contain the pattern pat
   (exactly pat_len digits).  It returns the count as seq_t. */
static seq_t count_leq(seq_t bound, const char *pat, int pat_len)
{
    /* 1. Convert bound into an array of decimal digits (most significant first). */
    int digits[40];
    int dlen = 0;
    if (bound == 0) {
        digits[0] = 0;
        dlen = 1;
    } else {
        seq_t tmp = bound;
        while (tmp > 0) {
            digits[dlen++] = (int)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to get most significant first */
        for (int i = 0; i < dlen / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[dlen - 1 - i];
            digits[dlen - 1 - i] = t;
        }
    }

    /* 2. Build transition table for the automaton that keeps the
       longest prefix of pat that is a suffix of the processed digits. */
    int trans[6][10];          /* pat_len ≤ 5  -> 6 slots for the terminal state */
    for (int k = 0; k <= pat_len; ++k) {
        for (int d = 0; d < 10; ++d) {
            if (k == pat_len) {
                trans[k][d] = pat_len;        /* once matched, stay matched */
                continue;
            }
            /* find longest l (≤ pat_len) such that the suffix of length l
               of (pat[0..k‑1] followed by d) equals pat[0..l‑1] */
            int best = 0;
            int maxl = (k + 1 < pat_len) ? k + 1 : pat_len;
            for (int l = maxl; l >= 0; --l) {
                bool ok = true;
                for (int j = 0; j < l; ++j) {
                    int srcIdx = k + 1 - l + j;
                    char ch = (srcIdx < k) ? pat[srcIdx] : ('0' + d);
                    if (ch != pat[j]) { ok = false; break; }
                }
                if (ok) { best = l; break; }
            }
            trans[k][d] = best;
        }
    }

    /* 3. DP arrays: only two layers are needed (current and next). */
    seq_t dp[2][2][2][6] = {0};   /* dp[layer][tight][started][matchLen] */
    int cur = 0, nxt = 1;
    dp[cur][1][0][0] = 1;         /* start: tight=1, started=0, matchLen=0 */

    for (int pos = 0; pos < dlen; ++pos) {
        /* reset next layer */
        for (int t = 0; t < 2; ++t)
            for (int s = 0; s < 2; ++s)
                for (int m = 0; m <= pat_len; ++m)
                    dp[nxt][t][s][m] = 0;

        int limit = digits[pos];
        for (int tight = 0; tight < 2; ++tight) {
            for (int started = 0; started < 2; ++started) {
                for (int m = 0; m <= pat_len; ++m) {
                    seq_t ways = dp[cur][tight][started][m];
                    if (!ways) continue;
                    int maxd = tight ? limit : 9;
                    for (int d = 0; d <= maxd; ++d) {
                        int tight2   = tight && (d == limit);
                        int started2 = started || (d != 0);
                        int m2;
                        if (!started2) {
                            m2 = 0;                      /* still no digits yet */
                        } else {
                            m2 = trans[m][d];
                        }
                        dp[nxt][tight2][started2][m2] += ways;
                    }
                }
            }
        }
        /* swap layers */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* 4. Sum all states that have started and matched the whole pattern. */
    seq_t result = 0;
    for (int tight = 0; tight < 2; ++tight) {
        result += dp[cur][tight][1][pat_len];
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* pattern = decimal string of n */
    char pat[20];
    int pat_len = num_to_str(n, pat);

    /* count numbers ≤ n² that contain the pattern */
    seq_t upper = count_leq((seq_t)n * (seq_t)n, pat, pat_len);

    /* count numbers ≤ n that contain the pattern */
    seq_t lower = count_leq((seq_t)n, pat, pat_len);

    return upper - lower;
}
