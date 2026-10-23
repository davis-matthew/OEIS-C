/*
 * A164503  Number of binary strings of length n with no substrings equal to
 *          0011 0101 or 0110.
 * https://oeis.org/A164503
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n * that avoid the
 *            substrings 0011, 0101, 0110. * This is computed by dynamic
 *            programming on a small * automaton that records the last up to 3
 *            bits of the * string
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 52, u64 n <= 110,
 *            u128 n <= 226, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include standard headers for types and bool */
#include <stddef.h>
#include <stdbool.h>

/* helper constants for the automaton */
#define MAX_STATES 15   /* 1 + 2 + 4 + 8 */
/* offsets for states of each length:
 * offset[0] = 0, offset[1] = 1, offset[2] = 3, offset[3] = 7
 */
static const int state_offset[4] = {0, 1, 3, 7};

/* transitional structures – computed only once */
static char trans_next[MAX_STATES][2];   /* next state id */
static bool trans_allowed[MAX_STATES][2];/* whether the transition is allowed */

/* initialization flag */
static bool automaton_built = false;

/* build the automaton the first time a() is called */
static void build_automaton(void)
{
    if (automaton_built) return;
    automaton_built = true;

    /* state properties: length and suffix (stored as int)   */
    int state_len[MAX_STATES];
    int state_suf[MAX_STATES];

    /* fill states array */
    for (int len = 0; len <= 3; ++len)
    {
        for (int suf = 0; suf < (1 << len); ++suf)
        {
            int id = state_offset[len] + suf;
            state_len[id]   = len;
            state_suf[id]   = suf;
        }
    }

    /* forbidden 4‑bit patterns encoded as integers */
    const int forb[3] = {3, 5, 6};   /* 0011, 0101, 0110 */

    for (int id = 0; id < MAX_STATES; ++id)
    {
        int len = state_len[id];
        int suf = state_suf[id];

        for (int b = 0; b <= 1; ++b)
        {
            bool allowed = true;
            int new_len, new_suf, new_id;

            if (len < 3)
            {
                /* still building the first few bits, no forbidden pattern can appear */
                new_len = len + 1;
                new_suf = ((suf << 1) | b) & ((1 << new_len) - 1);
            }
            else
            {
                /* len == 3: we must check the formed 4‑bit word */
                int four = ((suf << 1) | b) & 0xF;  /* top 4 bits, pattern length 4 */
                for (int i = 0; i < 3; ++i)
                {
                    if (four == forb[i]) { allowed = false; break; }
                }
                new_len = 3;
                new_suf = four & 0x7;               /* keep last 3 bits */
            }

            if (allowed)
                new_id = state_offset[new_len] + new_suf;
            else
                new_id = -1; /* unused */

            trans_next[id][b]   = (char)new_id;
            trans_allowed[id][b] = allowed;
        }
    }
}

/*--------------------------------------------------------------*/

seq_t a(idx_t n)
{
    /* build the automaton once */
    build_automaton();

    /* DP tables for the 15 states */
    seq_t dp[MAX_STATES]  = {0};
    seq_t ndp[MAX_STATES] = {0};

    dp[0] = (seq_t)1;   /* start with an empty string */

    for (idx_t i = 0; i < n; ++i)
    {
        /* zero ndp */
        for (int s = 0; s < MAX_STATES; ++s)
            ndp[s] = (seq_t)0;

        /* transition */
        for (int s = 0; s < MAX_STATES; ++s)
        {
            if (dp[s] == (seq_t)0) continue;
            for (int b = 0; b <= 1; ++b)
            {
                if (!trans_allowed[s][b]) continue;
                int ns = (int)trans_next[s][b];
                ndp[ns] += dp[s];
            }
        }

        /* copy ndp -> dp for next iteration */
        for (int s = 0; s < MAX_STATES; ++s)
            dp[s] = ndp[s];
    }

    /* sum all valid states to get the answer */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < MAX_STATES; ++s)
        ans += dp[s];

    return ans;
}
