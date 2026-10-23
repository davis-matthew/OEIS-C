/*
 * A143283  Number of binary words of length n containing at least one subword
 *          10001 and no subwords 10^{i}1 with i<3.
 * https://oeis.org/A143283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A003269(n+4) - A003520(n+4)
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 67, u64 n <= 136,
 *            u128 n <= 274, big any n
 * Helpers:   mask_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (computed as the number of binary words of length n
   containing a subword 10001 and no subword 10^i1 with i<3) */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mask_to_str.h"

/* check if pattern p of length plen occurs in string s of length slen */
static int contains_pattern(const char *s, size_t slen,
                            const char *p, size_t plen)
{
    if (plen > slen) return 0;
    for (size_t i = 0; i + plen <= slen; ++i) {
        int eq = 1;
        for (size_t j = 0; j < plen; ++j)
            if (s[i + j] != p[j]) { eq = 0; break; }
        if (eq) return 1;
    }
    return 0;
}

/* count of length n binary words avoiding all patterns in 'patterns' */
static seq_t count_avoid(idx_t n,
                         const char *patterns[], size_t patcnt,
                         size_t maxlen)
{
    /* generate all valid states (suffixes up to maxlen) */
    char state_str[64][6];
    size_t state_len[64];
    size_t states = 0;
    for (size_t len = 0; len <= maxlen; ++len) {
        for (uint32_t mask = 0; mask < (1u << len); ++mask) {
            char buf[6];
            size_t blen;
            mask_to_str(mask, len, buf, &blen);
            int bad = 0;
            for (size_t pi = 0; pi < patcnt; ++pi)
                if (contains_pattern(buf, blen,
                                     patterns[pi], strlen(patterns[pi])))
                    { bad = 1; break; }
            if (!bad) {
                memcpy(state_str[states], buf, blen + 1);
                state_len[states] = blen;
                ++states;
            }
        }
    }

    /* build transition table: -1 means dead state (forbidden) */
    int trans[64][2];
    memset(trans, -1, sizeof(trans));
    for (size_t s = 0; s < states; ++s) {
        for (int b = 0; b <= 1; ++b) {
            /* append bit b to current suffix */
            size_t slen = state_len[s];
            char newbuf[7];
            if (slen < maxlen) {
                memcpy(newbuf, state_str[s], slen);
                newbuf[slen] = '0' + b;
                newbuf[slen + 1] = '\0';
                slen = slen + 1;
            } else {
                /* shift left by 1 and add new bit */
                size_t shift = slen - 1;
                for (size_t i = 0; i < shift; ++i)
                    newbuf[i] = state_str[s][i + 1];
                newbuf[shift] = '0' + b;
                newbuf[shift + 1] = '\0';
                slen = shift + 1;
            }
            /* check if new suffix contains any forbidden pattern */
            int bad = 0;
            for (size_t pi = 0; pi < patcnt; ++pi)
                if (contains_pattern(newbuf, slen,
                                     patterns[pi], strlen(patterns[pi])))
                    { bad = 1; break; }
            if (bad) continue; /* dead, leave -1 */

            /* find the state index corresponding to newbuf */
            int tgt = -1;
            for (size_t t = 0; t < states; ++t)
                if (state_len[t] == slen &&
                    strncmp(state_str[t], newbuf, slen) == 0) {
                    tgt = (int)t;
                    break;
                }
            trans[s][b] = tgt;
        }
    }

    /* DP arrays */
    seq_t *dp = (seq_t*)calloc(states, sizeof(seq_t));
    seq_t *ndp = (seq_t*)calloc(states, sizeof(seq_t));
    if (!dp || !ndp) return 0; /* allocation failure */

    dp[0] = (seq_t)1; /* start with empty suffix */

    for (idx_t step = 0; step < n; ++step) {
        memset(ndp, 0, states * sizeof(seq_t));
        for (size_t s = 0; s < states; ++s) {
            if (dp[s] == 0) continue;
            for (int b = 0; b <= 1; ++b) {
                int t = trans[s][b];
                if (t >= 0) ndp[t] += dp[s];
            }
        }
        seq_t *tmp = dp; dp = ndp; ndp = tmp;
    }

    /* total count = sum over all states */
    seq_t total = 0;
    for (size_t s = 0; s < states; ++s) total += dp[s];

    free(dp);
    free(ndp);
    return total;
}

seq_t a(idx_t n)
{
    /* patterns for forbidden subwords 11, 101, 1001 */
    static const char *forbid[3] = {"11","101","1001"};
    /* patterns for forbidden subwords + the target 10001 */
    static const char *forbid_target[4] = {"11","101","1001","10001"};

    seq_t total = count_avoid(n, forbid, 3, 4);
    seq_t avoidboth = count_avoid(n, forbid_target, 4, 5);
    return total - avoidboth;
}
