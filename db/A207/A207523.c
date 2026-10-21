/*
 * A207523  Number of 7Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A207523
 *  Count of 7×n binary arrays avoiding horizontal 0 0 1 and 0 1 0
 *  and vertical 0 1 0 and 1 0 1.
 *
 *  The algorithm:
 *    * Enumerate all 7‑bit columns that contain no vertical forbidden
 *      pattern.  There are 42 such columns.
 *    * A state is an ordered pair of consecutive columns (a,b).
 *    * From a state (a,b) a new column c may follow iff, for every row,
 *      the 3‑cell horizontal pattern (a,b,c) is not 001 or 010.
 *      This gives a pre‑computed transition table.
 *    * For width n ≥ 2 we start with one configuration per state
 *      (count = 1).  Then we apply the transitions n−2 times.
 *    * The number of arrays of width n is the sum of the final
 *      state counts.
 *
 *  The code is fully generic in the integer types seq_t and idx_t
 *  supplied by the build.  It compiles as both C11 and C++17.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ROWS           7
#define ALLOWED_CNT    42               /* number of columns avoiding
                                           vertical patterns */
#define STATES         (ALLOWED_CNT * ALLOWED_CNT)

/* ---------- static data --------------------------------------------------- */
static uint8_t  allowed[ALLOWED_CNT];                /* list of allowed
                                                       columns */
static int      transCnt[STATES];                    /* number of
                                                       successors for
                                                       each state   */
static int      transTbl[STATES][ALLOWED_CNT];       /* successors   */
static bool     init_done = false;

/* ---------- helper: test a column for vertical forbidden patterns ----- */
static bool vcol_ok(uint8_t m)
{
    for (int i = 0; i <= ROWS - 3; ++i) {
        int b0 = (m >> i) & 1;
        int b1 = (m >> (i + 1)) & 1;
        int b2 = (m >> (i + 2)) & 1;
        if ((b0 == 0 && b1 == 1 && b2 == 0) ||
            (b0 == 1 && b1 == 0 && b2 == 1))
            return false;
    }
    return true;
}

/* ---------- initialise allowed columns and transition table ------------ */
static void init(void)
{
    if (init_done) return;

    /* build list of allowed columns */
    int cnt = 0;
    for (int m = 0; m < (1 << ROWS); ++m)
        if (vcol_ok((uint8_t)m))
            allowed[cnt++] = (uint8_t)m;
    /* there are exactly ALLOWED_CNT columns */
    (void)cnt;

    /* pre‑compute transitions */
    for (int i = 0; i < ALLOWED_CNT; ++i) {
        uint8_t a = allowed[i];
        for (int j = 0; j < ALLOWED_CNT; ++j) {
            uint8_t b = allowed[j];
            /* compute masks of rows forced to 0 or 1
               when we append a new column c. */
            int must0 = 0, must1 = 0;
            for (int r = 0; r < ROWS; ++r) {
                int bit_a = (a >> r) & 1;
                int bit_b = (b >> r) & 1;
                if (bit_a == 0 && bit_b == 0) must0 |= (1 << r);
                else if (bit_a == 0 && bit_b == 1) must1 |= (1 << r);
            }
            int idx = i * ALLOWED_CNT + j;
            int tcnt = 0;
            for (int k = 0; k < ALLOWED_CNT; ++k) {
                uint8_t c = allowed[k];
                if ( (c & must1) == must1 && (c & must0) == 0 )
                    transTbl[idx][tcnt++] = k;
            }
            transCnt[idx] = tcnt;
        }
    }
    init_done = true;
}

/* ---------- sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    init();
    if (n <= 1) return (seq_t)ALLOWED_CNT;               /* n=1: 42 */
    if (n == 2) return (seq_t)ALLOWED_CNT * (seq_t)ALLOWED_CNT; /* 1764 */

    /* DP arrays for current and next step */
    seq_t dp0[STATES];
    seq_t dp1[STATES];
    seq_t *pcur = dp0;
    seq_t *pnxt = dp1;

    /* initialise dp_cur for width 2: all state counts = 1 */
    for (size_t s = 0; s < (size_t)STATES; ++s)
        pcur[s] = (seq_t)1;

    idx_t steps = n - 2;   /* number of transitions to reach width n */
    for (idx_t step = 0; step < steps; ++step) {
        /* zero next array */
        memset(pnxt, 0, sizeof(dp1));

        for (size_t s = 0; s < (size_t)STATES; ++s) {
            seq_t val = pcur[s];
            if (val == 0) continue;
            int j = (int)(s % ALLOWED_CNT);   /* index of second column */
            int cnt = transCnt[s];
            if (cnt == 0) continue;
            const int *list = transTbl[s];
            for (int t = 0; t < cnt; ++t) {
                int k = list[t];
                size_t next_idx = (size_t)j * (size_t)ALLOWED_CNT + (size_t)k;
                pnxt[next_idx] += val;
            }
        }
        /* swap pointers for next iteration */
        seq_t *tmp = pcur;
        pcur = pnxt;
        pnxt = tmp;
    }

    /* sum all final states */
    seq_t ans = (seq_t)0;
    for (size_t s = 0; s < (size_t)STATES; ++s)
        ans += pcur[s];
    return ans;
}
