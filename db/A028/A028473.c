/*
 * A028473  Number of perfect matchings in graph P_{11} X P_{2n}.
 * https://oeis.org/A028473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculation: number of perfect matchings (domino tilings) of the
   11 × 2n grid P_{11} × P_{2n}.  The DP iterates over column states
   (a bitmask of length 11), using a pre‑computed transition table.
   The algorithm runs in O(n · 2^{11} · T) where T is the average
   number of transitions per state (≤ 144). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Pre‑computed transitions.  MAX_TRANS is the maximum number of
   transitions for any state (for a 1 × 11 column the number is 144). */
#define MAX_TRANS 200
#define STATES (1U << 11)

static uint16_t trans[STATES][MAX_TRANS];
static int      trans_len[STATES];
static bool     transitions_computed = false;

/* Recursive enumeration of all possible next column states for a
   given current state 'curMask'.
   pos      : current row being considered (0..10).
   curMask  : bits already occupied in this column.
   nextMask : bits that will be occupied in the next column by
              horizontal dominoes placed now.
   out      : output buffer to store resulting next masks.
   outCnt   : pointer to counter of produced masks. */
static void dfs_fill(uint16_t pos, uint16_t curMask, uint16_t nextMask,
                     uint16_t *out, int *outCnt)
{
    /* Skip already filled cells. */
    while (pos < 11 && (curMask & (1U << pos))) pos++;
    if (pos == 11) {
        out[(*outCnt)++] = nextMask;
        return;
    }

    /* Place a vertical domino covering pos and pos+1. */
    if (pos + 1 < 11 && !(curMask & (1U << (pos + 1))))
        dfs_fill(pos + 2,
                 curMask | (1U << pos) | (1U << (pos + 1)),
                 nextMask,
                 out, outCnt);

    /* Start a horizontal domino that will extend into the next column. */
    dfs_fill(pos + 1,
             curMask | (1U << pos),
             nextMask | (1U << pos),
             out, outCnt);
}

/* Compute the transition table once. */
static void init_transitions(void)
{
    if (transitions_computed) return;
    for (uint16_t s = 0; s < STATES; ++s) {
        int cnt = 0;
        dfs_fill(0, s, 0, trans[s], &cnt);
        trans_len[s] = cnt;
    }
    transitions_computed = true;
}

/* Main function computing the sequence term. */
seq_t a(idx_t n)
{
    /* Special case: empty board. */
    if (n == 0) return (seq_t)1;

    init_transitions();

    const idx_t width = n << 1;                  /* 2n columns */
    static seq_t dp[STATES];
    static seq_t nxt[STATES];

    /* Initialize dp[0] = 1, rest 0. */
    for (size_t i = 0; i < STATES; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t col = 0; col < width; ++col) {
        /* Clear next array. */
        for (size_t i = 0; i < STATES; ++i) nxt[i] = (seq_t)0;

        for (size_t s = 0; s < STATES; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;

            const int tlen = trans_len[s];
            const uint16_t *tarr = trans[s];
            for (int t = 0; t < tlen; ++t)
                nxt[tarr[t]] += ways;
        }

        /* Copy nxt to dp for next iteration. */
        for (size_t i = 0; i < STATES; ++i) dp[i] = nxt[i];
    }

    /* All horizontal dominoes must have been closed. */
    return dp[0];
}
