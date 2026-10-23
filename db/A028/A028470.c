/*
 * A028470  Number of perfect matchings in graph P_{8} X P_{n}.
 * https://oeis.org/A028470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of perfect matchings in P_8 × P_n (domino tilings of an 8
 *            × n rectangle)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Transition matrices for the 8‑row transfer matrix */
static uint8_t   M_len[256];        /* number of non‑zero transitions for each state */
static uint8_t   M_next[256][256];  /* destination state indices */
static uint32_t  M_cnt [256][256];  /* small transition counts (fit in 32 bits) */
static bool      M_ready = false;

/* Recursive helper to generate transitions for a single start mask */
static void dfs_transition(int pos, int curMask, int nextMask, int temp[256])
{
    if (pos == 8) {
        /* all rows processed – record resulting nextMask */
        temp[nextMask] += 1;
        return;
    }
    if (curMask & (1 << pos)) {
        /* current cell already filled – skip to next */
        dfs_transition(pos + 1, curMask, nextMask, temp);
        return;
    }
    /* Try placing a vertical domino if possible */
    if (pos + 1 < 8 && !(curMask & (1 << (pos + 1)))) {
        int newMask = curMask | (1 << pos) | (1 << (pos + 1));
        dfs_transition(pos + 2, newMask, nextMask, temp);
    }
    /* Try placing a horizontal domino to the next column */
    int newMask  = curMask | (1 << pos);
    int newNext  = nextMask | (1 << pos);
    dfs_transition(pos + 1, newMask, newNext, temp);
}

/* Build the transfer matrix once */
static void build_transitions(void)
{
    if (M_ready) return;
    M_ready = true;
    for (int s = 0; s < 256; ++s) {
        int temp[256] = {0};
        dfs_transition(0, s, 0, temp);
        uint8_t len = 0;
        for (int t = 0; t < 256; ++t) if (temp[t]) {
            M_next[s][len] = (uint8_t)t;
            M_cnt [s][len] = (uint32_t)temp[t];
            ++len;
        }
        M_len[s] = len;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    build_transitions();

    seq_t cur[256] = {0};
    seq_t nxt[256];

    cur[0] = 1;          /* empty state at column 0 */

    for (idx_t col = 1; col <= n; ++col) {
        for (int i = 0; i < 256; ++i) nxt[i] = 0;

        for (int s = 0; s < 256; ++s) if (cur[s] != 0) {
            uint8_t len = M_len[s];
            for (uint8_t k = 0; k < len; ++k) {
                int t   = M_next[s][k];
                seq_t add = cur[s] * (seq_t)M_cnt[s][k];
                nxt[t] += add;
            }
        }

        for (int i = 0; i < 256; ++i) cur[i] = nxt[i];
    }

    return cur[0];  /* all cells matched after n columns */
}
