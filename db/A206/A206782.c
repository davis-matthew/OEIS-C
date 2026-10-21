/*
 * A206782  Number of nX5 0..1 arrays avoiding 0 0 1 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via dynamic programming over row states
   (rows satisfy horizontal avoidance of 001, columns avoid 011 vertically). */
#include <stdbool.h>
#include <stdint.h>

/* Helper to test horizontal (001) pattern in a 5‑bit row */
static bool ok_row(uint8_t r)
{
    /* r is a 5‑bit number, bits 4..0 are the row cells */
    uint8_t b0 = (r >> 0) & 1;
    uint8_t b1 = (r >> 1) & 1;
    uint8_t b2 = (r >> 2) & 1;
    uint8_t b3 = (r >> 3) & 1;
    uint8_t b4 = (r >> 4) & 1;
    /* check all 3‑bit substrings */
    if (b0 == 0 && b1 == 0 && b2 == 1) return false;
    if (b1 == 0 && b2 == 0 && b3 == 1) return false;
    if (b2 == 0 && b3 == 0 && b4 == 1) return false;
    return true;
}

/* Number of allowed rows (20) */
#define RCOUNT 20
/* Max states: pair of rows */
#define STATECOUNT (RCOUNT * RCOUNT)

/* Global static data, initialized once */
static bool initialized = false;
static uint8_t rows[RCOUNT];                     /* valid 5‑bit rows */
static int next_row[STATECOUNT][RCOUNT];         /* next rows allowed for each state */
static int next_len[STATECOUNT];                 /* number of allowed next rows */

static void init_data(void)
{
    if (initialized) return;
    /* Enumerate all 5‑bit rows avoiding horizontal 001 */
    int pos = 0;
    for (uint8_t r = 0; r < 32; ++r)
    {
        if (ok_row(r))
        {
            rows[pos++] = r;
        }
    }
    /* pos should be RCOUNT (20) */
    for (int i = 0; i < RCOUNT; ++i)
    {
        for (int j = 0; j < RCOUNT; ++j)
        {
            int state = i * RCOUNT + j;
            next_len[state] = 0;
            uint8_t a = rows[i];
            uint8_t b = rows[j];
            /* iterate over all possible new rows k */
            for (int k = 0; k < RCOUNT; ++k)
            {
                uint8_t c = rows[k];
                bool ok = true;
                /* check for each column the vertical pattern 011 */
                for (int col = 0; col < 5; ++col)
                {
                    uint8_t bitA = (a >> col) & 1;
                    uint8_t bitB = (b >> col) & 1;
                    uint8_t bitC = (c >> col) & 1;
                    if (bitA == 0 && bitB == 1 && bitC == 1)
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok)
                {
                    next_row[state][next_len[state]++] = k;
                }
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init_data();
    /* Special cases for n = 1, 2 */
    if (n == 1) return (seq_t)RCOUNT; /* 20 */
    if (n == 2) return (seq_t)RCOUNT * (seq_t)RCOUNT; /* 400 */

    /* DP arrays */
    static seq_t dp[STATECOUNT];
    static seq_t dp_next[STATECOUNT];
    /* Initialize dp for the first two rows */
    for (int i = 0; i < STATECOUNT; ++i) dp[i] = 0;
    for (int i = 0; i < RCOUNT; ++i)
    {
        for (int j = 0; j < RCOUNT; ++j)
        {
            dp[i * RCOUNT + j] = 1;
        }
    }

    /* Iterate rows 3..n */
    for (idx_t row = 3; row <= n; ++row)
    {
        for (int i = 0; i < STATECOUNT; ++i) dp_next[i] = 0;
        for (int i = 0; i < STATECOUNT; ++i)
        {
            seq_t cur = dp[i];
            if (cur == 0) continue;
            int len = next_len[i];
            int j = i % RCOUNT; /* previous row index */
            for (int t = 0; t < len; ++t)
            {
                int k = next_row[i][t];
                int new_state = j * RCOUNT + k;
                dp_next[new_state] += cur;
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < STATECOUNT; ++i) dp[i] = dp_next[i];
    }

    /* Sum all DP states to get total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < STATECOUNT; ++i) total += dp[i];
    return total;
}
