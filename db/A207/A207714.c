/*
 * A207714  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×5 0..1 arrays avoiding the horizontal *
 *            patterns 000 and 001 and the vertical patterns 001 and 100. *
 *            Computation by dynamic programming over pairs of consecutive
 *            rows
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* allowed 5‑bit rows (decimal representation of the binary string) */
static const uint8_t rows[16] = {
    10,11,12,13,14,15,
    20,21,22,23,26,27,
    28,29,30,31
};

/* compatibility table: compat[p][q][r] is true iff rows p,q,r can
 * appear consecutively (vertical restriction 001/100).  The table is
 * built once. */
static bool compat[16][16][16];
static bool compat_ready = false;

static void build_compat( void )
{
    for (int p = 0; p < 16; ++p) {
        for (int q = 0; q < 16; ++q) {
            for (int r = 0; r < 16; ++r) {
                bool ok = true;
                for (int j = 0; j < 5 && ok; ++j) {
                    unsigned qbit = (rows[q] >> j) & 1U;
                    if (qbit == 0U) {
                        unsigned pbit = (rows[p] >> j) & 1U;
                        unsigned rbit = (rows[r] >> j) & 1U;
                        if (pbit != rbit) ok = false;
                    }
                }
                compat[p][q][r] = ok;
            }
        }
    }
    compat_ready = true;
}

seq_t a( idx_t n )
{
    /* fast answers for the first two terms */
    if (n == 1U) return (seq_t)16U;
    if (n == 2U) return (seq_t)256U;

    if (!compat_ready) build_compat();

    /* dp stores the number of sequences of the current length ending
     * with the last two rows (indexed as p*16+q).  Initially, for
     * length 2, every pair of allowed rows occurs once. */
    seq_t dp[256];
    seq_t next[256];
    for (int i = 0; i < 256; ++i) dp[i] = (seq_t)1U;

    for (idx_t len = 3U; len <= n; ++len) {
        for (int i = 0; i < 256; ++i) next[i] = (seq_t)0U;
        for (int idx = 0; idx < 256; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;
            int p = idx / 16;   /* previous‑previous row */
            int q = idx % 16;   /* previous row          */
            for (int r = 0; r < 16; ++r) {
                if (!compat[p][q][r]) continue;
                int ni = (q << 4) | r;
                next[ni] = next[ni] + val;
            }
        }
        /* move next into dp for the next iteration */
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    /* total number of sequences of length n is the sum of all dp entries */
    seq_t total = (seq_t)0U;
    for (int i = 0; i < 256; ++i) total = total + dp[i];
    return total;
}
