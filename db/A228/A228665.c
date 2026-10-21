/*
 * A228665  Number of 6 X n binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  DP computation of OEIS A228665:
    Number of 6×n binary arrays with top left value 1 and no two ones
    adjacent horizontally, diagonally or antidiagonally.
    The sequence is computed by iterating over all 6‑bit column masks,
    enforcing the adjacency rules between consecutive columns.  The top
    row of the first column is forced to 1.  Results are cached so that
    repeated calls are O(1).  The implementation is O(n·64²) time and
    O(1) extra space besides the cache. */

#include <stdlib.h>
#include <stdbool.h>

#ifndef MAX_N
#define MAX_N 20000     /* supports n up to ~10,000 in the tests */
#endif

/*  The build system supplies the following types:                     */
/*  seq_t  – an unsigned integer type (8/16/32/64 bit or big integer).   */
/*  idx_t  – an unsigned integer type (uint64_t).                       */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. */
    if (n < 1) return 0;

    /* Cache for already computed terms.  Elements 1..MAX_N are valid. */
    static seq_t cache[MAX_N + 1];
    static idx_t last_n = 0;            /* largest n that has been built */

    /* Compatibility table: compat[prev][curr] == 1
       iff column mask 'curr' can follow column mask 'prev'. */
    static unsigned char compat[64][64];
    static bool compat_inited = false;
    if (!compat_inited) {
        unsigned char forb[64];
        for (int curr = 0; curr < 64; ++curr) {
            unsigned char bits = 0;
            for (int r = 0; r < 6; ++r) if (curr & (1 << r)) {
                if (r > 0) bits |= 1 << (r - 1);
                bits |= 1 << r;
                if (r < 5) bits |= 1 << (r + 1);
            }
            forb[curr] = bits;
        }
        for (int prev = 0; prev < 64; ++prev)
            for (int curr = 0; curr < 64; ++curr)
                compat[prev][curr] = ((prev & forb[curr]) == 0) ? 1 : 0;
        compat_inited = true;
    }

    /* DP arrays for current and previous column masks. */
    static seq_t dp_prev[64], dp_curr[64];
    static bool dp_inited = false;
    if (!dp_inited) {
        /* Initial column 0: top row must be 1 (bit 0). */
        for (int m = 0; m < 64; ++m)
            dp_prev[m] = (m & 1) ? 1 : 0;

        /* a(1) = sum over all admissible masks for the first column. */
        seq_t total = 0;
        for (int m = 0; m < 64; ++m) total += dp_prev[m];
        cache[1] = total;
        last_n = 1;
        dp_inited = true;
    }

    if (n > last_n) {
        /* Extend DP to the requested n. */
        for (idx_t col = last_n; col < n; ++col) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t sum = 0;
                for (int prev = 0; prev < 64; ++prev)
                    if (compat[prev][curr])
                        sum += dp_prev[prev];
                dp_curr[curr] = sum;
            }
            seq_t total = 0;
            for (int curr = 0; curr < 64; ++curr) total += dp_curr[curr];
            cache[col + 1] = total;
            for (int curr = 0; curr < 64; ++curr)
                dp_prev[curr] = dp_curr[curr];
        }
        last_n = n;
    }

    return cache[n];
}
