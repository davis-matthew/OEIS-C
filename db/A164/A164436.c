/*
 * A164436  Number of binary strings of length n with no substrings equal to
 *          0000 0101 or 1110.
 * https://oeis.org/A164436
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 48, u64 n <= 99, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS A164436, which counts binary
   strings of length n that avoid the forbidden substrings
   0000, 0101 and 1110.  It uses a 9‑state DFA (empty and all prefixes
   of length ≤3 of the forbidden patterns).  The DP counts the valid
   strings for each length.  The routine works for any n ≥ 4 and
   does not assume increasing calls. */

#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* do nothing for indices below the offset */
    if (n < 4) return 0;

    /* automaton tables, built once */
    static bool built = false;
    static int  trans[9][2];
    static bool bad[9][2];

    if (!built) {
        /* prefixes of the forbidden patterns   */
        static const char *pref[9] = {
            /* 0 */ "",    /* 1 */ "0",   /* 2 */ "00",   /*3*/ "000",
            /* 4 */ "01",  /* 5 */ "010", /*6*/ "1",    /*7*/ "11",
            /* 8 */ "111"
        };
        static int pref_len[9] = {0,1,2,3,2,3,1,2,3};

        /* forbidden patterns */
        static const char forb[3][5] = {"0000", "0101", "1110"};

        for (int s = 0; s < 9; ++s) {
            int plen = pref_len[s];
            const char *ps = pref[s];
            for (int bit = 0; bit < 2; ++bit) {
                /* form state string + new bit */
                char tmp[5];              /* max 3+1 chars + '\0' */
                memcpy(tmp, ps, plen);
                tmp[plen] = (bit ? '1' : '0');
                tmp[plen + 1] = '\0';
                int tlen = plen + 1;

                /* check if just formed a forbidden pattern */
                bool isbad = false;
                if (tlen >= 4) {
                    for (int f = 0; f < 3 && !isbad; ++f) {
                        if (strncmp(tmp + tlen - 4, forb[f], 4) == 0)
                            isbad = true;
                    }
                }
                bad[s][bit] = isbad;

                if (!isbad) {
                    /* longest suffix that is a prefix of some forbidden pattern */
                    int best = 0;   /* empty string state */
                    int maxlen = (tlen < 3) ? tlen : 3;
                    for (int l = maxlen; l >= 1; --l) {
                        const char *suf = tmp + tlen - l;
                        for (int p = 0; p < 9; ++p) {
                            if (pref_len[p] == l &&
                                strncmp(suf, pref[p], l) == 0) {
                                best = p;
                                goto found;
                            }
                        }
                    }
                found:
                    trans[s][bit] = best;
                } else {
                    trans[s][bit] = 0; /* not used */
                }
            }
        }
        built = true;
    }

    /* DP arrays: number of ways to reach each state after current length */
    seq_t dp_cur[9] = {0};
    seq_t dp_next[9];
    dp_cur[0] = 1;          /* empty string starts at state 0 */

    for (idx_t len = 1; len <= n; ++len) {
        /* zero next array */
        for (int s = 0; s < 9; ++s) dp_next[s] = 0;

        for (int s = 0; s < 9; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            for (int bit = 0; bit < 2; ++bit) {
                if (bad[s][bit]) continue;
                int ns = trans[s][bit];
                dp_next[ns] += val;
            }
        }
        /* copy next to current for next iteration */
        for (int s = 0; s < 9; ++s) dp_cur[s] = dp_next[s];
    }

    /* sum over all valid ending states to get a(n) */
    seq_t result = 0;
    for (int s = 0; s < 9; ++s) result += dp_cur[s];
    return result;
}
