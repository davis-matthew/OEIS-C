/*
 * A207238  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary matrices avoiding horizontal 000 and
 *            011 and vertical 001 and 010. Computed by dynamic programming
 *            over rows
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* ---------- precomputation ---------- */
    static bool initialized = false;
    static bool valid_row[16];
    static uint16_t adj[256]; /* bitmask of allowed third rows */

    if (!initialized) {
        /* horizontal validity of each 4‑bit row */
        for (int r = 0; r < 16; ++r) {
            bool ok = true;
            for (int pos = 0; pos < 2; ++pos) {          /* two length‑3 windows */
                int trip = (r >> pos) & 7;                /* 3 bits */
                if (trip == 0 || trip == 3) {            /* 000 or 011 forbidden */
                    ok = false;
                    break;
                }
            }
            valid_row[r] = ok;
        }

        /* adjacency: for each pair (r1,r2) compute allowed r3 */
        for (int r1 = 0; r1 < 16; ++r1) {
            for (int r2 = 0; r2 < 16; ++r2) {
                uint16_t mask = 0;
                if (!valid_row[r1] || !valid_row[r2]) {
                    adj[(r1 << 4) | r2] = 0;
                    continue;
                }
                for (int r3 = 0; r3 < 16; ++r3) {
                    if (!valid_row[r3])
                        continue;
                    bool good = true;
                    for (int col = 0; col < 4; ++col) {
                        int b1 = (r1 >> col) & 1;
                        int b2 = (r2 >> col) & 1;
                        int b3 = (r3 >> col) & 1;
                        if ((b1 == 0 && b2 == 0 && b3 == 1)   /* 001 */
                         || (b1 == 0 && b2 == 1 && b3 == 0)) /* 010 */
                        {
                            good = false;
                            break;
                        }
                    }
                    if (good)
                        mask |= (1u << r3);
                }
                adj[(r1 << 4) | r2] = mask;
            }
        }
        initialized = true;
    }

    /* Handle trivial cases explicitly */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)9;
    if (n == 2)
        return (seq_t)81;

    /* ---------- DP arrays ----------
       dp[state] = number of arrays ending with rows (r1,r2) where
       state = (r1 << 4) | r2                          */
    static seq_t dp[256];
    static seq_t dp_next[256];

    /* reset DP for current calculation */
    for (int i = 0; i < 256; ++i)
        dp[i] = (seq_t)0;

    /* initialise for n = 2: all pairs of valid rows */
    for (int r1 = 0; r1 < 16; ++r1) {
        if (!valid_row[r1])
            continue;
        for (int r2 = 0; r2 < 16; ++r2) {
            if (!valid_row[r2])
                continue;
            dp[(r1 << 4) | r2] = (seq_t)1;
        }
    }

    /* extend rows from 3 up to n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 256; ++i)
            dp_next[i] = (seq_t)0;

        for (int state = 0; state < 256; ++state) {
            seq_t ways = dp[state];
            if (ways == (seq_t)0)
                continue;

            uint16_t mask = adj[state];
            if (mask == 0)
                continue;

            int r2 = state & 0xF;  /* r2 is the last row of the current state */

            for (int r3 = 0; r3 < 16; ++r3) {
                if (!(mask & (1u << r3)))
                    continue;
                int new_state = (r2 << 4) | r3;
                dp_next[new_state] += ways;
            }
        }

        /* copy dp_next into dp for next iteration */
        for (int i = 0; i < 256; ++i)
            dp[i] = dp_next[i];
    }

    /* sum all ways for length n */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 256; ++i)
        total += dp[i];

    return total;
}
