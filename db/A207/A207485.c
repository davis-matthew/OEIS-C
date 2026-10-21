/*
 * A207485  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n×5 binary arrays with no horizontal 000 or 011 and
 *            no vertical 011 or 110. The function uses a linear‑time
 *            dynamic‑programming algorithm over all admissible 5‑bit rows
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_ROWS 32
#define MAX_STATES (MAX_ROWS * MAX_ROWS)

/* Storage for enumeration of admissible rows and transitions. */
static int  good_rows[MAX_ROWS];
static int  G = 0;
static unsigned char trans_next[MAX_STATES][MAX_ROWS];
static unsigned char trans_count[MAX_STATES];
static seq_t dp_prev[MAX_STATES];
static seq_t dp_curr[MAX_STATES];
static bool  initialized = false;

/* Test whether a 5‑bit row contains no 000 or 011 horizontally. */
static bool good_row(uint8_t mask)
{
    /* there are exactly three 3‑bit windows in a 5-bit string */
    for (int pos = 0; pos < 3; ++pos) {
        int a = (mask >> pos)     & 1;
        int b = (mask >> (pos+1)) & 1;
        int c = (mask >> (pos+2)) & 1;
        if ((a == 0 && b == 0 && c == 0) ||
            (a == 0 && b == 1 && c == 1))
            return false;
    }
    return true;
}

/* Test vertical constraint for a triple of rows. */
static bool vertical_ok(uint8_t a, uint8_t b, uint8_t c)
{
    for (int pos = 0; pos < 5; ++pos) {
        int a_bit = (a >> pos) & 1;
        int b_bit = (b >> pos) & 1;
        int c_bit = (c >> pos) & 1;
        if ((a_bit == 0 && b_bit == 1 && c_bit == 1) ||
            (a_bit == 1 && b_bit == 1 && c_bit == 0))
            return false;
    }
    return true;
}

/* One‑time initialization of good rows and transition tables. */
static void init(void)
{
    if (initialized) return;

    /* Enumerate all 5‑bit rows that satisfy horizontal constraints. */
    for (int m = 0; m < 32; ++m) {
        if (good_row((uint8_t)m)) {
            good_rows[G++] = m;
        }
    }

    /* Build transition lists for each pair of rows. */
    for (int i = 0; i < G; ++i) {
        for (int j = 0; j < G; ++j) {
            int s = i * G + j;
            trans_count[s] = 0;
            for (int k = 0; k < G; ++k) {
                if (vertical_ok(good_rows[i], good_rows[j], good_rows[k])) {
                    trans_next[s][trans_count[s]++] = (unsigned char)k;
                }
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init();

    if (n == 0) return (seq_t)0;   /* below the offset is unused */

    if (n == 1) return (seq_t)G;               /* single row: any good row */
    if (n == 2) return (seq_t)G * (seq_t)G;    /* two rows: no vertical restriction */

    const int S = G * G;

    /* DP for rows 3 … n.  dp_prev holds counts for the last two rows. */
    for (int s = 0; s < S; ++s) dp_prev[s] = (seq_t)1;

    for (idx_t row = 3; row <= n; ++row) {
        /* reset next state array */
        for (int s = 0; s < S; ++s) dp_curr[s] = (seq_t)0;

        for (int s = 0; s < S; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;

            int j = s % G;          /* second row of the current state */
            unsigned char cnt = trans_count[s];

            for (unsigned char t = 0; t < cnt; ++t) {
                unsigned char k = trans_next[s][t];
                int s2 = j * G + k; /* shift state window: (prev1, next) */
                dp_curr[s2] += val;
            }
        }

        /* copy dp_curr to dp_prev for next iteration */
        for (int s = 0; s < S; ++s) dp_prev[s] = dp_curr[s];
    }

    /* Sum over all ending states to get the final count. */
    seq_t res = (seq_t)0;
    for (int s = 0; s < G * G; ++s) res += dp_prev[s];
    return res;
}
