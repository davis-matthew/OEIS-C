/*
 * A183319  Number of n X 7 binary arrays with an element zero only if there
 *          are an even number of ones to its left and an even number of ones
 *          above it.
 * https://oeis.org/A183319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A183319: Number of n × 7 binary arrays with an element zero only if
   there are an even number of ones to its left and an even number of
   ones above it.
   Implementation: Dynamic programming on column‑parity states.
   States are 7‑bit vectors (128 states). For each state we precompute
   how many row patterns transform it to each next state.  The count for
   n rows is obtained by powering this transition matrix.
*/
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* transition counts: T[s][sp] = number of row patterns that map
       column‑parity state s to sp.  Size 128 × 128 for width 7.   */
    static unsigned char T[128][128];
    static bool initialized = false;
    if (!initialized) {
        for (int s = 0; s < 128; ++s)
            for (int sp = 0; sp < 128; ++sp)
                T[s][sp] = 0;

        for (int s = 0; s < 128; ++s) {
            uint8_t up[7];
            for (int j = 0; j < 7; ++j)
                up[j] = (s >> j) & 1;

            for (int p = 0; p < 128; ++p) {
                int lpar = 0;
                bool ok = true;
                for (int j = 0; j < 7; ++j) {
                    int bit = (p >> j) & 1;
                    if (bit == 0) {
                        if (lpar != 0 || up[j] != 0) { ok = false; break; }
                    }
                    lpar ^= bit;
                }
                if (!ok) continue;
                int ns = s ^ p;          /* update parity of columns   */
                T[s][ns]++;              /* count this row pattern     */
            }
        }
        initialized = true;
    }

    seq_t cur[128] = {0};
    seq_t nxt[128] = {0};
    cur[0] = 1;                     /* start with no ones above   */

    for (idx_t r = 0; r < n; ++r) {
        /* reset next array */
        for (int j = 0; j < 128; ++j) nxt[j] = 0;

        /* transition from each state */
        for (int s = 0; s < 128; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            for (int sp = 0; sp < 128; ++sp) {
                unsigned char t = T[s][sp];
                if (t == 0) continue;
                nxt[sp] += val * t;  /* add all equivalent rows   */
            }
        }

        /* move nxt -> cur for next iteration */
        for (int j = 0; j < 128; ++j) cur[j] = nxt[j];
    }

    /* sum all ending states to get the number of matrices */
    seq_t ans = 0;
    for (int s = 0; s < 128; ++s) ans += cur[s];
    return ans;
}
