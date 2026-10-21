/*
 * A206990  Number of nX4 0..1 arrays avoiding 0 0 0 horizontally and 1 1 1
 *          vertically.
 * https://oeis.org/A206990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206990 – number of n×4 binary arrays avoiding 000
   horizontally and 111 vertically.  The computation is done by a
   dynamic‑programming traversal of the 81 possible column‑run states.
   This routine works for any build where seq_t and idx_t are defined
   as required.  No global memory is freed. */

#include <stdint.h>
#include <stdbool.h>

/* The state of a column is represented by a value in base 3:
   0 → no trailing 1s, 1 → one trailing 1, 2 → two trailing 1s.
   Four columns give 3^4 = 81 possible states.  */
#define STATE_COUNT 81
#define MAX_TRANS   13

static bool    ready = false;
static uint8_t trans[STATE_COUNT][MAX_TRANS];
static uint8_t transCnt[STATE_COUNT];
static uint8_t stateRun[STATE_COUNT][4];

/* Precompute the transition table once. */
static void init(void)
{
    if (ready) return;
    /* Enumerate all 16 4‑bit rows that avoid a horizontal 000. */
    uint8_t rows[ MAX_TRANS ];
    int rowCnt = 0;
    for (int r = 0; r < 16; ++r) {
        if ( ((r & 0x7) == 0) || ((r >> 1) & 0x7) == 0 ) continue;
        rows[rowCnt++] = (uint8_t)r;
    }

    /* Decode each state into the per‑column run lengths. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int tmp = s;
        for (int j = 0; j < 4; ++j) {
            stateRun[s][j] = tmp % 3;
            tmp /= 3;
        }
    }

    const int pow3[4] = {1,3,9,27};

    /* Build transition table: from state s using allowed row r
       produce next state t (run lengths updated). */
    for (int s = 0; s < STATE_COUNT; ++s) {
        transCnt[s] = 0;
        for (int k = 0; k < rowCnt; ++k) {
            uint8_t r = rows[k];
            int t = 0;
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                uint8_t run   = stateRun[s][j];
                uint8_t bit   = (r >> j) & 1U;
                if (bit && run == 2) { ok = false; break; }
                uint8_t next  = bit ? (run + 1) : 0U;
                t += next * pow3[j];
            }
            if (ok) trans[s][transCnt[s]++] = (uint8_t)t;
        }
    }
    ready = true;
}

/* Return a(n).  For n == 0 the routine returns 0, otherwise the
   number of suitable n×4 arrays. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init();   /* Ensure transition table exists. */

    /* DP vectors (one element per state).  Use local arrays to avoid
       interference between calls.  seq_t may be 8/16/32/64/128‑bit or
       arbitrary‑precision. */
    seq_t dp[STATE_COUNT]  = {0};
    seq_t nextdp[STATE_COUNT] = {0};

    dp[0] = (seq_t)1;  /* start from state with all run lengths 0 */

    for (idx_t step = 0; step < n; ++step) {
        /* Zero nextdp. */
        for (int i = 0; i < STATE_COUNT; ++i) nextdp[i] = (seq_t)0;

        /* Transition from each active state. */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cur = dp[s];
            if (cur == (seq_t)0) continue;
            for (int tIdx = 0; tIdx < transCnt[s]; ++tIdx) {
                int t = trans[s][tIdx];
                nextdp[t] += cur;
            }
        }
        /* Swap dp and nextdp for next iteration. */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = nextdp[i];
    }

    /* Sum all final states. */
    seq_t res = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i) res += dp[i];
    return res;
}
