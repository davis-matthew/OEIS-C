/*
 * A203403  Number of (n+2)X6 binary arrays avoiding patterns 000 and 111 in
 *          rows and columns.
 * https://oeis.org/A203403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A203403 : number of (n+2)×6 binary arrays avoiding
    patterns 000 and 111 in every row and column.
    The algorithm uses a transfer‑matrix on the last two rows.
    Horizontal constraint is enforced by enumerating all 6‑bit
    patterns that avoid 000/111.  For the vertical constraint,
    the last two rows determine the allowed next rows independently
    for each column.  The DP runs in O(n·S·deg) with S=4096 and
    deg≈3, fast enough for n up to 10000. */

#include <stdint.h>
#include <stdbool.h>

static const uint8_t ALLOWED_BITS = 6;          /* width of each row */

/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    /* number of rows in the matrix: (n+2) */
    idx_t m = n + 2;
    idx_t steps = m - 2;          /* number of additional rows after
                                      the first two rows               */

    /* ---------- static initialisation -------------------------------- */
    static bool initialized = false;
    static uint8_t allowed[64];
    static uint8_t allowed_cnt = 0;

    /* transitions: for each state (prev2<<6 | prev1) we store the
       list of next states after appending a valid row   */
    static uint8_t  next_cnt[1 << 12];
    static uint16_t next_states[1 << 12][64];

    if (!initialized) {
        /* enumerate allowed rows (no 000 or 111 horizontally) */
        for (int pat = 0; pat < 64; ++pat) {
            bool ok = true;
            for (int j = 2; j < 6; ++j) {
                int b2 = (pat >> (j - 2)) & 1;
                int b1 = (pat >> (j - 1)) & 1;
                int b  = (pat >> j) & 1;
                if (b2 == b1 && b1 == b) { ok = false; break; }
            }
            if (ok) {
                allowed[allowed_cnt++] = (uint8_t)pat;
            }
        }

        /* build transitions */
        for (int s = 0; s < (1 << 12); ++s) {
            uint8_t prev1 = s & 63;      /* last row            */
            uint8_t prev2 = s >> 6;      /* second last row     */
            uint8_t eq_bits  = (~(prev1 ^ prev2)) & 63;     /* columns where bits equal */
            uint8_t must_val = (~prev1) & eq_bits;          /* required bits     */

            next_cnt[s] = 0;
            for (uint8_t k = 0; k < allowed_cnt; ++k) {
                uint8_t r = allowed[k];
                if ( (r & eq_bits) == must_val ) {
                    uint16_t t = ((uint16_t)prev1 << 6) | r;
                    next_states[s][next_cnt[s]++] = t;
                }
            }
        }
        initialized = true;
    }

    /* DP arrays */
    static seq_t dp[1 << 12];
    static seq_t ndp[1 << 12];

    /* initialise dp with all pairs of allowed rows (first two rows) */
    for (int i = 0; i < (1 << 12); ++i) dp[i] = 0;
    for (uint8_t i = 0; i < allowed_cnt; ++i) {
        for (uint8_t j = 0; j < allowed_cnt; ++j) {
            uint16_t s = ((uint16_t)allowed[i] << 6) | allowed[j];
            dp[s] = dp[s] + (seq_t)1;
        }
    }

    /* DP iterations for the remaining rows */
    for (idx_t step = 0; step < steps; ++step) {
        for (int i = 0; i < (1 << 12); ++i) ndp[i] = 0;
        for (int s = 0; s < (1 << 12); ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            uint8_t deg = next_cnt[s];
            for (uint8_t k = 0; k < deg; ++k) {
                uint16_t t = next_states[s][k];
                ndp[t] = ndp[t] + val;
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < (1 << 12); ++i) dp[i] = ndp[i];
    }

    /* sum all states to get the total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < (1 << 12); ++i) {
        total = total + dp[i];
    }
    return total;
}
