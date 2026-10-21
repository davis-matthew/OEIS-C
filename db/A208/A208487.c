/*
 * A208487  Number of (n+1) X 4 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to the number of
 *          counterclockwise edge increases.
 * https://oeis.org/A208487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0,1,2,3} such that * for
 *            every 2×2 subblock the number of clockwise edge increases equals
 *            * the number of counter‑clockwise edge increases (computed by
 *            DP)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Precomputed transitions between rows of length 4 over {0,1,2,3}. */
static uint8_t row_digits[256][4];
static uint16_t outdeg[256];
static uint16_t dest[256][256];

/* DP state for current and next row counts. */
static seq_t dp_cur[256];
static seq_t dp_next[256];

/* Cache for already computed a(n).  n is never > MAXCACHE in tests. */
#define MAXCACHE 100000
static seq_t cache[MAXCACHE + 1];
static idx_t cached_n = 0;          /* largest n that has been cached */
static bool adjacency_built = false;

/* Build adjacency graph between rows. */
static void build_adjacency(void)
{
    for (int i = 0; i < 256; ++i) {
        row_digits[i][0] = (i >> 6) & 3;
        row_digits[i][1] = (i >> 4) & 3;
        row_digits[i][2] = (i >> 2) & 3;
        row_digits[i][3] =  i        & 3;
    }

    for (int i = 0; i < 256; ++i) {
        outdeg[i] = 0;
        for (int j = 0; j < 256; ++j) {
            bool ok = true;
            for (int c = 0; c < 3 && ok; ++c) {
                int a   = row_digits[i][c];
                int b   = row_digits[i][c+1];
                int c1  = row_digits[j][c];
                int d   = row_digits[j][c+1];

                int cw  = (a < b) + (b < d) + (d < c1) + (c1 < a);
                int ccw = (a < c1) + (c1 < d) + (d < b) + (b < a);
                if (cw != ccw) ok = false;
            }
            if (ok) {
                dest[i][outdeg[i]++] = (uint16_t)j;
            }
        }
    }
    adjacency_built = true;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Initialise if first call. */
    if (!adjacency_built) {
        build_adjacency();
        for (int i = 0; i < 256; ++i) dp_cur[i] = 1;   /* paths of length 1 */
        cached_n = 0;
        cache[0] = 0;
    }

    if (n <= cached_n) return cache[(size_t)n];

    for (idx_t step = cached_n + 1; step <= n; ++step) {
        for (int j = 0; j < 256; ++j) dp_next[j] = 0;

        for (int i = 0; i < 256; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            uint16_t od = outdeg[i];
            for (uint16_t k = 0; k < od; ++k) {
                uint16_t j = dest[i][k];
                dp_next[(int)j] += val;
            }
        }

        /* Sum of all paths ending in any row gives a(step). */
        seq_t sum = 0;
        for (int j = 0; j < 256; ++j) sum += dp_next[j];

        /* Cache the result.  (step <= MAXCACHE by problem statement). */
        cache[(size_t)step] = sum;
        cached_n = step;

        /* Prepare for next iteration. */
        for (int j = 0; j < 256; ++j) dp_cur[j] = dp_next[j];
    }

    return cache[(size_t)n];
}
