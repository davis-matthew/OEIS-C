/*
 * A164460  Number of binary strings of length n with no substrings equal to
 *          0001 0011 or 1101.
 * https://oeis.org/A164460
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) counts binary strings of length n that contain none of the
 *            forbidden substrings 0001, 0011, or 1101. The implementation
 *            uses a deterministic finite automaton with states that are the
 *            suffixes of length ≤ 3 of already constructed strings,
 *            performing a linear DP over the length
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 51, u64 n <= 106,
 *            u128 n <= 215, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* All suffixes of length at most 3 that may appear in a valid string. */
static const char * const STATES[] = {
    "",   "0",   "1",
    "00", "01", "10", "11",
    "000","001","010","011",
    "100","101","110","111"
};
#define STATE_COUNT (sizeof(STATES)/sizeof(STATES[0]))

/* Transition table: for each state and next bit, the resulting state
   index, or -1 if the transition creates a forbidden pattern. */
static int trans[STATE_COUNT][2];
static int initialized = 0;

/* Convert a suffix string (length ≤ 3) to its state index. */
static int state_id(const char *s)
{
    for (int i = 0; i < STATE_COUNT; ++i) {
        if (strcmp(STATES[i], s) == 0) return i;
    }
    return -1; /* should never happen for valid inputs */
}

/* Build the transition table once. */
static void init_transitions(void)
{
    if (initialized) return;
    for (int s = 0; s < STATE_COUNT; ++s) {
        const char *cur = STATES[s];
        size_t cur_len = strlen(cur);
        for (int bit = 0; bit <= 1; ++bit) {
            /* Candidate string cur + bit */
            char tmp[5]; /* max length 4 + '\0' */
            memcpy(tmp, cur, cur_len);
            tmp[cur_len] = '0' + bit;
            tmp[cur_len + 1] = '\0';

            /* Reject if it creates any forbidden 4‑bit pattern */
            if (cur_len == 3) {
                if ((tmp[0]=='0' && tmp[1]=='0' && tmp[2]=='0' && tmp[3]=='1') ||
                    (tmp[0]=='0' && tmp[1]=='0' && tmp[2]=='1' && tmp[3]=='1') ||
                    (tmp[0]=='1' && tmp[1]=='1' && tmp[2]=='0' && tmp[3]=='1')) {
                    trans[s][bit] = -1;
                    continue;
                }
            }

            /* Determine the new state (suffix of the new string, up to length 3) */
            const char *next;
            if (cur_len < 3) {
                next = tmp; /* length ≤ 3 */
            } else {
                next = tmp + 1; /* last 3 characters of the 4‑bit string */
            }
            trans[s][bit] = state_id(next);
        }
    }
    initialized = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* behaviour below offset is irrelevant */

    init_transitions();

    seq_t dp[STATE_COUNT];
    seq_t newdp[STATE_COUNT];

    /* Initialize dp: empty string starts in state 0 */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Extend the string one bit at a time up to length n */
    for (idx_t len = 0; len < n; ++len) {
        for (int i = 0; i < STATE_COUNT; ++i) newdp[i] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (dp[s] == 0) continue;
            for (int bit = 0; bit <= 1; ++bit) {
                int ns = trans[s][bit];
                if (ns == -1) continue;                  /* forbidden transition */
                newdp[ns] = newdp[ns] + dp[s];
            }
        }
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = newdp[i];
    }

    /* Sum counts over all terminal states */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total = total + dp[i];
    return total;
}
