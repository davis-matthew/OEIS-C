/*
 * A080654  Smallest number with exactly n representations as a sum of five
 *          positive squares or 0 if no such number exists (cf. A025429).
 * https://oeis.org/A080654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 9361, u32 n <= 9361, u64 n <= 9361,
 *            u128 n <= 9361, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A080654: smallest number with exactly n representations
   as a sum of five positive squares.  The routine builds a DP table
   counting unordered partitions of an integer into five squares, then
   records the first integer that achieves each representation count up to
   10000. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TARGET 10000        /* maximal n we need to support */
#define MAX_SUM     40000        /* upper bound on numbers to be examined */

/* DP table: dp[p][s] = number of ways to write s as sum of p non‑decreasing
   positive squares, capped at MAX_TARGET+1. */
static uint16_t dp[6][MAX_SUM + 1];

/* Storage of answers: ans[k] = smallest number with exactly k representations
   (k in [1,MAX_TARGET]); ans[0] unused. */
static seq_t ans[MAX_TARGET + 1] = {0};

/* One‑time initialisation flag. */
static int initialized = 0;

/* ---------------  initialise DP and compute answers --------------- */
static void init(void)
{
    if (initialized)
        return;
    initialized = 1;

    /* dp[0][0] = 1.  All other entries are already zero. */
    dp[0][0] = 1;

    /* Iterate over all square values in increasing order. */
    for (int v = 1; v * v <= MAX_SUM; ++v) {
        int sq = v * v;
        for (int p = 1; p <= 5; ++p) {                /* number of parts used */
            for (int s = sq; s <= MAX_SUM; ++s) {     /* current sum */
                uint16_t add = dp[p - 1][s - sq];
                if (add) {
                    uint32_t nv = (uint32_t)dp[p][s] + add;
                    if (nv > MAX_TARGET + 1)
                        nv = MAX_TARGET + 1;
                    dp[p][s] = (uint16_t)nv;
                }
            }
        }
    }

    /* Record, for each representation count, the first sum that achieved it. */
    for (int s = 1; s <= MAX_SUM; ++s) {
        uint16_t cnt = dp[5][s];
        if (cnt >= 1 && cnt <= MAX_TARGET && ans[cnt] == (seq_t)0)
            ans[cnt] = (seq_t)s;
    }
}

/* --------------------------  public function  -------------------------- */
seq_t a(idx_t n)
{
    if (n == 0 || n > MAX_TARGET)
        return (seq_t)0;      /* out of requested range or offset */
    init();                  /* ensure tables are ready */

    return ans[(idx_t)n];
}
