/*
 * A164488  Number of binary strings of length n with no substrings equal to
 *          0001 1010 or 1101.
 * https://oeis.org/A164488
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 49, u64 n <= 100,
 *            u128 n <= 203, big any n
 * Helpers:   state_id, state_len, state_suf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/state_id.h"
#include "../utils/state_len.h"
#include "../utils/state_suf.h"

/* Computes the number of binary strings of length n with no substrings
   equal to 0001, 1010 or 1101.  The implementation uses a deterministic
   finite automaton with 15 states (0–14) corresponding to the last up to
   three bits seen.  DP over the states yields the desired count. */

#define STATE_COUNT 15  /* 1 state of length 0, 2 of length 1, 4 of length 2, 8 of length 3 */

/* Helpers to convert between state id and (length, suffix). */

seq_t a(idx_t n)
{
    /* Forbidden 4‑bit patterns (in decimal). */
    const int forbid1 = 1;   /* 0001 */
    const int forbid2 = 10;  /* 1010 */
    const int forbid3 = 13;  /* 1101 */

    /* Transition table. */
    int next_state[STATE_COUNT][2];
    bool ok_trans[STATE_COUNT][2];

    /* Build transition tables. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int len_s = state_len(s);
        int suf_s = state_suf(s);
        for (int b = 0; b <= 1; ++b) {
            bool allowed = true;
            if (len_s == 3) {
                int cand = (suf_s << 1) | b;          /* 4‑bit candidate */
                if (cand == forbid1 || cand == forbid2 || cand == forbid3)
                    allowed = false;
            }
            ok_trans[s][b] = allowed;
            if (allowed) {
                int new_len = (len_s < 3) ? len_s + 1 : 3;
                int new_suf = ((suf_s << 1) | b) & ((1 << new_len) - 1);
                next_state[s][b] = state_id(new_len, new_suf);
            } else {
                next_state[s][b] = -1;     /* unused value */
            }
        }
    }

    /* DP arrays: only two rows needed. */
    seq_t prev[STATE_COUNT] = {0};
    seq_t cur[STATE_COUNT]  = {0};

    prev[0] = 1;    /* start from empty string */

    for (idx_t i = 0; i < n; ++i) {
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = prev[s];
            if (ok_trans[s][0]) {
                int ns = next_state[s][0];
                cur[ns] = cur[ns] + val;
            }
            if (ok_trans[s][1]) {
                int ns = next_state[s][1];
                cur[ns] = cur[ns] + val;
            }
        }
        /* Move to next step: copy cur to prev and clear cur. */
        for (int s = 0; s < STATE_COUNT; ++s) {
            prev[s] = cur[s];
            cur[s]  = 0;
        }
    }

    /* Sum over all valid ending states. */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s)
        result = result + prev[s];

    return result;
}
