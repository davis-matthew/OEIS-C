/*
 * A133523  Number of length n binary sequences with at most 2 of every
 *          adjacent 6 bits set.
 * https://oeis.org/A133523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 49, u64 n <= 99, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP over 5‑bit suffixes: count binary strings of length n such that
 * every consecutive block of 6 bits (including trailing zeros that
 * lie beyond position n-1) contains at most two 1’s.
 *
 * This satisfies the empirical recurrence (see OEIS A133523) and
 * reproduces the listed terms (a(0)=1, a(1)=2, a(2)=4, a(3)=7, …).
 * The algorithm runs in O(n) time and uses a fixed 32‑element table. */
#include <stdbool.h>

/* pre‑computed popcount for 5‑bit masks */
static seq_t popcnt[32];
static bool popcnt_ready = false;

seq_t a(idx_t n)
{
    /* initialise popcount table once */
    if (!popcnt_ready) {
        for (idx_t m = 0; m < 32; ++m) {
            seq_t c = 0;
            for (idx_t b = 0; b < 5; ++b)
                c += (m >> b) & 1;
            popcnt[m] = c;
        }
        popcnt_ready = true;
    }

    /* DP arrays: cur and nxt hold counts for each 5‑bit mask */
    seq_t cur[32] = {0}, nxt[32] = {0};
    cur[0] = 1;                     /* start with empty string */

    for (idx_t pos = 0; pos < n; ++pos) {
        /* clear nxt */
        for (idx_t m = 0; m < 32; ++m) nxt[m] = 0;

        for (idx_t m = 0; m < 32; ++m) {
            seq_t val = cur[m];
            if (!val) continue;

            for (seq_t bit = 0; bit <= 1; ++bit) {
                /* if a full 6‑bit window would be formed,
                   check the number of 1’s in the previous 5 bits plus the new bit. */
                if (pos >= 5 && popcnt[m] + bit > 2) continue;

                idx_t newm = ((m << 1) & 0x1F) | (idx_t)bit;
                nxt[newm] += val;
            }
        }
        /* move to next position */
        for (idx_t m = 0; m < 32; ++m) cur[m] = nxt[m];
    }

    /* after building n bits, ensure trailing zeros meet the
       constraint: the last 5 bits (ending at position n-1) must contain ≤2 ones. */
    seq_t total = 0;
    for (idx_t m = 0; m < 32; ++m)
        if (popcnt[m] <= 2)
            total += cur[m];
    return total;
}
