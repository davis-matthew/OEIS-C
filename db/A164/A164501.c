/*
 * A164501  Number of binary strings of length n with no substrings equal to
 *          0010 1001 or 1100.
 * https://oeis.org/A164501
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 83, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164501: Number of binary strings of length n with no substrings 0010, 1001, 1100
   Implemented by a DFA over the longest suffixes of the forbidden patterns. */

#include <stdbool.h>

static const char *states[] = {
    "",    /* 0: empty */
    "0",   /* 1 */
    "1",   /* 2 */
    "00",  /* 3 */
    "10",  /* 4 */
    "11",  /* 5 */
    "001", /* 6 */
    "100", /* 7 */
    "110"  /* 8 */
};

static const unsigned state_len[] = {0,1,1,2,2,2,3,3,3};
static const char *forbidden[] = {"0010", "1001", "1100"};
static const unsigned patterns = 3;
enum { STATE_COUNT = 9 };

static int trans[STATE_COUNT][2] = {0};
static bool trans_built = false;

static void build_transitions(void)
{
    for (unsigned s = 0; s < STATE_COUNT; ++s) {
        for (unsigned b = 0; b < 2; ++b) {
            unsigned l = state_len[s];
            char buf[5];                         /* max 4 bits + NUL */
            for (unsigned i = 0; i < l; ++i) buf[i] = states[s][i];
            buf[l] = (char)('0' + b);
            buf[l + 1] = '\0';
            unsigned newlen = l + 1;

            /* Forbidden check: if a full 4-bit string is formed, reject if it matches any forbidden pattern */
            bool bad = false;
            if (newlen == 4) {
                for (unsigned f = 0; f < patterns; ++f) {
                    if (buf[0] == forbidden[f][0] &&
                        buf[1] == forbidden[f][1] &&
                        buf[2] == forbidden[f][2] &&
                        buf[3] == forbidden[f][3]) {
                        bad = true;
                        break;
                    }
                }
            }
            if (bad) {
                trans[s][b] = -1;
                continue;
            }

            /* Find the longest suffix of buf that is a prefix of any forbidden pattern (i.e., a state) */
            int best = 0;
            unsigned bestlen = 0;
            for (unsigned j = 0; j < STATE_COUNT; ++j) {
                unsigned slen = state_len[j];
                if (slen > newlen) continue;
                bool match = true;
                for (unsigned k = 0; k < slen; ++k) {
                    if (buf[newlen - slen + k] != states[j][k]) {
                        match = false;
                        break;
                    }
                }
                if (match && slen > bestlen) {
                    bestlen = slen;
                    best = (int)j;
                }
            }
            trans[s][b] = best;
        }
    }
}

seq_t a(idx_t n)
{
    if (!trans_built) {
        build_transitions();
        trans_built = true;
    }

    /* Dynamic programming over DFA states */
    seq_t cur[STATE_COUNT] = {0};
    seq_t nxt[STATE_COUNT];
    cur[0] = (seq_t)1;            /* empty string */

    for (unsigned i = 0; i < n; ++i) {
        for (unsigned j = 0; j < STATE_COUNT; ++j) nxt[j] = (seq_t)0;
        for (unsigned s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            for (unsigned b = 0; b < 2; ++b) {
                int t = trans[s][b];
                if (t == -1) continue;
                nxt[t] += val;
            }
        }
        for (unsigned j = 0; j < STATE_COUNT; ++j) cur[j] = nxt[j];
    }

    seq_t total = 0;
    for (unsigned j = 0; j < STATE_COUNT; ++j) total += cur[j];
    return total;
}
