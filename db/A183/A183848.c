/*
 * A183848  Number of (n+1)X4 0..3 arrays with each element of every 2X2
 *          subblock being the sum mod 4 of two others.
 * https://oeis.org/A183848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0,1,2,3} where in every
 *            2×2 subblock each element equals the sum modulo 4 of two other
 *            elements in the same subblock
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming over rows.  */
#include <stdbool.h>

static void decode_row(unsigned int id, unsigned char vals[4])
{
    /* row encoded in base‑4: lowest two bits are element 0, etc. */
    for (int p = 0; p < 4; ++p)
        vals[p] = (unsigned char)((id >> (2 * p)) & 3U);
}

/* test whether a 2×2 block satisfies the property */
static bool block_ok(const unsigned char vals[4])
{
    /* vals[0..3] represent the 2×2 block:
       [ vals[0] (top‑left)  vals[1] (top‑right) ]
       [ vals[2] (bottom‑left) vals[3] (bottom‑right) ]
    */
    for (int idx = 0; idx < 4; ++idx) {
        bool ok = false;
        for (int p = 0; p < 4; ++p) {
            if (p == idx) continue;
            for (int q = p + 1; q < 4; ++q) {
                if (q == idx) continue;
                if ( ((int)vals[p] + (int)vals[q]) % 4 == vals[idx] ) {
                    ok = true;
                    break;
                }
            }
            if (ok) break;
        }
        if (!ok) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static data for one‑time initialization */
    static bool initialized = false;
    static unsigned char neigh[256][256];
    static unsigned char deg[256];
    static bool compat[256][256];

    if (!initialized) {
        /* build compatibility table for all 256 rows */
        unsigned char row1[4], row2[4];
        for (int i = 0; i < 256; ++i) {
            decode_row(i, row1);
            deg[i] = 0;
            for (int j = 0; j < 256; ++j) {
                decode_row(j, row2);
                bool ok = true;
                for (int sub = 0; sub < 3 && ok; ++sub) {
                    unsigned char vals[4] = {
                        row1[sub],     row1[sub + 1],
                        row2[sub],     row2[sub + 1]
                    };
                    if (!block_ok(vals))
                        ok = false;
                }
                compat[i][j] = ok;
                if (ok) {
                    neigh[i][deg[i]++] = (unsigned char)j;
                }
            }
        }
        initialized = true;
    }

    /* iterative DP over rows */
    static seq_t cur[256];
    static seq_t next[256];
    for (int i = 0; i < 256; ++i)
        cur[i] = (seq_t)1;   /* start with a single row */

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 256; ++i)
            next[i] = (seq_t)0;

        for (int i = 0; i < 256; ++i) {
            seq_t val = cur[i];
            if (val == (seq_t)0) continue;
            unsigned char d = deg[i];
            unsigned char *nb = neigh[i];
            for (unsigned char k = 0; k < d; ++k)
                next[nb[k]] += val;
        }

        for (int i = 0; i < 256; ++i)
            cur[i] = next[i];
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < 256; ++i)
        total += cur[i];

    return total;
}
