/*
 * A223983  Number of nX4 0..3 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A223983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 86, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes number of n×4 arrays with rows unimodal and columns nondecreasing. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* OEIS sequence A223983: a(n) = number of n×4 arrays
   with rows unimodal (first non‑decreasing then non‑increasing)
   and columns nondecreasing.                         */

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Build tables describing all 4‑digit rows over {0,1,2,3}:
       -- val[idx][j] : the j‑th digit of state idx (0…255)
       -- unimodal[idx] : is this row unimodal ?
       -- neigh[t][k]   : list of rows s such that s <= t elementwise
       ------------------------------------------------------------------ */
    static bool initialized = false;
    static unsigned char val[256][4];
    static bool unimodal[256];
    static unsigned char neigh[256][256];
    static int neigh_cnt[256];

    if (!initialized) {
        /* fill digit table */
        for (idx_t idx = 0; idx < 256; ++idx) {
            unsigned v = (unsigned)idx;
            for (int j = 0; j < 4; ++j) {
                val[idx][j] = v & 3;
                v >>= 2;
            }
        }

        /* determine unimodal rows */
        for (idx_t idx = 0; idx < 256; ++idx) {
            bool ok = false;
            for (int peak = 0; peak < 4; ++peak) {
                bool inc = true, dec = true;
                for (int i = 0; i < peak; ++i)
                    if (val[idx][i] > val[idx][i + 1]) inc = false;
                for (int i = peak; i < 3; ++i)
                    if (val[idx][i] < val[idx][i + 1]) dec = false;
                if (inc && dec) { ok = true; break; }
            }
            unimodal[idx] = ok;
        }

        /* build neighbour lists: all s such that s <= t elementwise */
        for (int t = 0; t < 256; ++t) {
            int cnt = 0;
            if (!unimodal[t]) { neigh_cnt[t] = 0; continue; }
            for (int s = 0; s < 256; ++s) {
                if (!unimodal[s]) continue;
                bool le = true;
                for (int j = 0; j < 4; ++j)
                    if (val[s][j] > val[t][j]) { le = false; break; }
                if (le) neigh[t][cnt++] = (unsigned char)s;
            }
            neigh_cnt[t] = cnt;
        }

        initialized = true;
    }

    /* ------------------------------------------------------------------
       Dynamic programming over rows.
       dp_prev[t] = number of arrays ending with row state t
       for the current number of processed rows.
       ------------------------------------------------------------------ */
    static seq_t dp_prev[256];
    static seq_t dp_next[256];

    /* first row */
    for (int t = 0; t < 256; ++t)
        dp_prev[t] = unimodal[t] ? (seq_t)1 : (seq_t)0;

    seq_t total = 0;
    for (int t = 0; t < 256; ++t) total += dp_prev[t];
    if (n == 1) return total;

    /* add rows 2 .. n */
    for (idx_t step = 2; step <= n; ++step) {
        for (int t = 0; t < 256; ++t) {
            if (!unimodal[t]) { dp_next[t] = 0; continue; }
            seq_t sum = 0;
            int cnt = neigh_cnt[t];
            for (int i = 0; i < cnt; ++i)
                sum += dp_prev[neigh[t][i]];
            dp_next[t] = sum;
        }

        /* copy to dp_prev for next iteration */
        for (int t = 0; t < 256; ++t) dp_prev[t] = dp_next[t];

        /* update running total */
        total = 0;
        for (int t = 0; t < 256; ++t) total += dp_prev[t];
    }

    return total;
}
