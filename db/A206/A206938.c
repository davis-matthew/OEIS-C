/*
 * A206938  Number of 5Xn 0..1 arrays avoiding 0 0 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over columns using a finite‑state machine.
   States correspond to the trailing zero length (0,1,2) in each of the
   five rows (3³ = 243 states).  A transition is possible if the
   vertical column pattern does not contain 011 and the horizontal
   update does not produce a forbidden 000.  The number of 5×n arrays
   is the sum of DP values after processing n columns. */
#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 243   /* 3^5 */
#define PATTERN_COUNT 32  /* 5-bit columns */

static int16_t nextState[STATE_COUNT][PATTERN_COUNT];
static bool     initialized = false;

/* Precompute whether a 5‑bit column pattern contains a forbidden
   vertical pattern 011 and the resulting state transition table. */
static void init_globals(void)
{
    /* Pre‑compute validity of all 5‑bit patterns. */
    bool validPattern[PATTERN_COUNT];
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        bool ok = true;
        for (int i = 0; i <= 2; ++i) {
            if (((p >> i) & 1) == 0 &&
                ((p >> (i + 1)) & 1) == 1 &&
                ((p >> (i + 2)) & 1) == 1) {
                ok = false;
                break;
            }
        }
        validPattern[p] = ok;
    }

    /* Calculate transitions for every state and pattern. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int stateDigits[5];
        int tmp = s;
        for (int r = 0; r < 5; ++r) {
            stateDigits[r] = tmp % 3;
            tmp /= 3;
        }

        for (int p = 0; p < PATTERN_COUNT; ++p) {
            if (!validPattern[p]) {
                nextState[s][p] = -1;
                continue;
            }

            int newState = 0;
            int multiplier = 1;
            bool ok = true;
            for (int r = 0; r < 5; ++r) {
                int bit = (p >> r) & 1;
                int cur = stateDigits[r];
                if (cur == 2 && bit == 0) { /* would form 000 */
                    ok = false;
                    break;
                }
                int nxt = (bit == 1) ? 0 : cur + 1;
                newState += nxt * multiplier;
                multiplier *= 3;
            }

            nextState[s][p] = ok ? newState : -1;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_globals();
        initialized = true;
    }

    /* DP lanes for current and next column. */
    seq_t dp[STATE_COUNT];
    seq_t nxt[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;
    dp[0] = 1;   /* all rows start with state 0 (no trailing zeros) */

    for (idx_t col = 0; col < n; ++col) {
        for (int i = 0; i < STATE_COUNT; ++i) nxt[i] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            for (int p = 0; p < PATTERN_COUNT; ++p) {
                int ns = nextState[s][p];
                if (ns >= 0) {
                    nxt[ns] += val;
                }
            }
        }
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = nxt[i];
    }

    /* Sum over all states to get the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) {
        total += dp[i];
    }
    return total;
}
