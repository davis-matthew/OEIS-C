/*
 * A164467  Number of binary strings of length n with no substrings equal to
 *          0001 0100 or 1101.
 * https://oeis.org/A164467
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 51, u64 n <= 104,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of binary strings of length n that avoid
 * the substrings 0001, 0100, or 1101.  The sequence begins
 * a(4)=13, a(5)=20, …
 *
 * Computation: build an Aho‑Corasick automaton for the three forbidden
 * patterns and run a linear DP over the automaton states.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static const char *prefixes[] = {
    "",      /* 0: root           */
    "0",     /* 1                 */
    "00",    /* 2                 */
    "000",   /* 3                 */
    "0001",  /* 4: forbidden     */
    "01",    /* 5                 */
    "010",   /* 6                 */
    "0100",  /* 7: forbidden     */
    "1",     /* 8                 */
    "11",    /* 9                 */
    "110",   /*10                 */
    "1101"   /*11: forbidden     */
};
#define STATE_COUNT (sizeof(prefixes)/sizeof(prefixes[0]))
static int prefix_len[STATE_COUNT];

static int next_state[STATE_COUNT][2];
static bool terminal[STATE_COUNT];
static bool initialized = false;

/* linear search for the id of a prefix string */
static int find_prefix_id(const char *s, int len)
{
    for (int id = 0; id < STATE_COUNT; ++id) {
        if (prefix_len[id] != len) continue;
        if (strncmp(prefixes[id], s, len) == 0) return id;
    }
    return -1;
}

/* initialise transition table and terminal flags once */
static void init(void)
{
    for (int i = 0; i < STATE_COUNT; ++i)
        prefix_len[i] = (int)strlen(prefixes[i]);

    /* terminal states are the full forbidden patterns */
    for (int i = 0; i < STATE_COUNT; ++i)
        terminal[i] = false;
    terminal[4] = true; /* 0001 */
    terminal[7] = true; /* 0100 */
    terminal[11] = true;/* 1101 */

    /* build next_state table */
    for (int s = 0; s < STATE_COUNT; ++s) {
        const char *ps = prefixes[s];
        int pslen = prefix_len[s];
        for (int bit = 0; bit < 2; ++bit) {
            /* construct T = ps + bit */
            char T[10];          /* max pattern length 4 + 1 */
            memcpy(T, ps, pslen);
            T[pslen] = (bit == 0 ? '0' : '1');
            int Tlen = pslen + 1;

            /* find longest suffix of T that is a prefix of any pattern */
            int best_id = 0;  /* default root */
            int maxk = (Tlen < 4 ? Tlen : 4);
            for (int k = maxk; k >= 0; --k) {
                int id = find_prefix_id(T + (Tlen - k), k);
                if (id >= 0) {
                    best_id = id;
                    break;
                }
            }
            next_state[s][bit] = best_id;
        }
    }
    initialized = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n < 4) return 0; /* offset is 4; behaviour below is irrelevant */

    static seq_t dp[STATE_COUNT];
    static seq_t ndp[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t len = 1; len <= n; ++len) {
        for (int i = 0; i < STATE_COUNT; ++i) ndp[i] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (terminal[s] || dp[s] == 0) continue;
            for (int bit = 0; bit < 2; ++bit) {
                int t = next_state[s][bit];
                if (terminal[t]) continue;
                ndp[t] += dp[s];
            }
        }
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = ndp[i];
    }

    seq_t res = 0;
    for (int s = 0; s < STATE_COUNT; ++s)
        if (!terminal[s]) res += dp[s];
    return res;
}
