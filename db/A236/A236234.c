/*
 * A236234  Number of binary words of length n with exactly 5 (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n.
 * https://oeis.org/A236234
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary words of length n that contain exactly 5
 *            occurrences (overlapping allowed) of the binary representation
 *            of n
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 41, u64 n <= 75, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

static const int K = 5;   /* required number of occurrences */

/* Return the number of binary words of length n that contain exactly K
   occurrences of the pattern equal to the binary representation of n.  */
seq_t a(idx_t n)
{
    /* Compute the bit length L of n. */
    idx_t tmp = n;
    int L = 0;
    while (tmp > 0) {
        ++L;
        tmp >>= 1;
    }
    if (L == 0) return 0; /* not used for n >= 7 */

    /* Pattern array p[0..L-1] (most significant bit first). */
    unsigned int p[32];   /* L <= 14 for n <= 10000, reserve more for safety */
    for (int i = 0; i < L; ++i) {
        p[i] = (unsigned int)((n >> (L - 1 - i)) & 1);
    }

    /* Prefix function (KMP). */
    int pi[32];
    pi[0] = 0;
    for (int i = 1; i < L; ++i) {
        int j = pi[i - 1];
        while (j > 0 && p[i] != p[j]) j = pi[j - 1];
        if (p[i] == p[j]) ++j;
        pi[i] = j;
    }
    int fail = pi[L - 1];   /* state to jump to after a full match */

    /* Transition table and match increment. */
    int trans[32][2];
    int inc[32][2];
    for (int s = 0; s < L; ++s) {
        for (int b = 0; b < 2; ++b) {
            int k = s;
            while (k > 0 && b != p[k]) k = pi[k - 1];
            if (b == p[k]) ++k;
            if (k == L) {            /* a match is found */
                inc[s][b] = 1;
                trans[s][b] = fail;
            } else {
                inc[s][b] = 0;
                trans[s][b] = k;
            }
        }
    }

    /* DP arrays: dp[s*(K+1)+cnt] stores the number of words reaching
       state s with exactly cnt matches. */
    size_t dim = (size_t)L * (K + 1);

#ifdef __cplusplus
    std::vector<seq_t> dp_cur(dim);
    std::vector<seq_t> dp_next(dim);
    dp_cur[0] = 1; /* start at state 0 with 0 matches */
#else
    seq_t *dp_cur = calloc(dim, sizeof(seq_t));
    seq_t *dp_next = calloc(dim, sizeof(seq_t));
    if (!dp_cur || !dp_next) { free(dp_cur); free(dp_next); return 0; }
    dp_cur[0] = 1; /* start at state 0 with 0 matches */
#endif

    for (idx_t pos = 0; pos < n; ++pos) {
#ifdef __cplusplus
        std::fill(dp_next.begin(), dp_next.end(), 0);
#else
        for (size_t i = 0; i < dim; ++i) dp_next[i] = 0;
#endif

        for (int s = 0; s < L; ++s) {
            for (int cnt = 0; cnt <= K; ++cnt) {
                seq_t val = dp_cur[s * (K + 1) + cnt];
                if (!val) continue;
                for (int b = 0; b < 2; ++b) {
                    int ns = trans[s][b];
                    int ncnt = cnt + inc[s][b];
                    if (ncnt <= K) {
                        dp_next[ns * (K + 1) + ncnt] += val;
                    }
                }
            }
        }

#ifdef __cplusplus
        std::swap(dp_cur, dp_next);
#else
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
#endif
    }

    /* Sum all ways that end with exactly K matches. */
    seq_t result = 0;
    for (int s = 0; s < L; ++s) {
        result += dp_cur[s * (K + 1) + K];
    }

#ifndef __cplusplus
    free(dp_cur);
    free(dp_next);
#endif
    return result;
}
