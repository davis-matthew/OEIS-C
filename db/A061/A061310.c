/*
 * A061310  Numbers which cannot be reached in the Countdown Numbers Game
 *          starting from (1,2,3,4,5,6): the game allows brackets and the
 *          operations + - x and / and not all numbers need be used, while the
 *          result of each partial calculation must be an integer.
 * https://oeis.org/A061310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer that cannot be obtained by the Countdown
 *            Numbers Game using (1,2,3,4,5,6) and operations +,-,*,/. All
 *            intermediate results must be integers. All numbers greater than
 *            1080 are impossible, so for n beyond the count of impossible
 *            numbers ≤1080 the sequence is simply consecuti...
 * Range:     u8 none, u16 n <= 538, u32 n <= 538, u64 n <= 538, u128 n <= 538,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NUM      6
#define FULLMASK ((1 << NUM) - 1)
#define OFFSET   5000          /* shift for negative indices */
#define BOUND    5000          /* we only keep results in [-BOUND, BOUND] */
#define WIDTH    (2 * BOUND + 1)

static bool initialized = false;
static int  impossible[1081];          /* impossible numbers ≤1080   */
static size_t impossible_cnt = 0;       /* how many such numbers exist */

/* DP tables: for each subset we store reachable integer values
   as a bitset and a list of those values for efficient iteration.  */
static unsigned char dp[1 << NUM][WIDTH];
static int  values[1 << NUM][WIDTH];   /* store values in the bitset order   */
static size_t valcnt[1 << NUM] = {0};

static void build_tables(void)
{
    /* initialize singletons */
    for (int i = 0; i < NUM; ++i) {
        int mask = 1 << i;
        int val  = i + 1;                 /* numbers are 1..6 */
        dp[mask][val + OFFSET] = 1;
        values[mask][valcnt[mask]++] = val;
    }

    /* build all subsets */
    for (int mask = 1; mask <= FULLMASK; ++mask) {
        if ((mask & (mask - 1)) == 0) continue; /* already singletons */

        /* iterate over non‑empty proper submasks */
        for (int sub = (mask - 1) & mask; sub; sub = (sub - 1) & mask) {
            int A = sub;
            int B = mask ^ sub;
            /* avoid duplicate partitions by requiring A < B */
            if (A >= B) continue;

            /* iterate over all values of A and B */
            for (size_t ai = 0; ai < valcnt[A]; ++ai) {
                int x = values[A][ai];
                for (size_t bi = 0; bi < valcnt[B]; ++bi) {
                    int y = values[B][bi];

                    /* + */
                    int res = x + y;
                    if (res >= -BOUND && res <= BOUND &&
                        !dp[mask][res + OFFSET]) {
                        dp[mask][res + OFFSET] = 1;
                        values[mask][valcnt[mask]++] = res;
                    }
                    /* - (x - y) */
                    res = x - y;
                    if (res >= -BOUND && res <= BOUND &&
                        !dp[mask][res + OFFSET]) {
                        dp[mask][res + OFFSET] = 1;
                        values[mask][valcnt[mask]++] = res;
                    }
                    /* - (y - x) */
                    res = y - x;
                    if (res >= -BOUND && res <= BOUND &&
                        !dp[mask][res + OFFSET]) {
                        dp[mask][res + OFFSET] = 1;
                        values[mask][valcnt[mask]++] = res;
                    }
                    /* * */
                    res = x * y;
                    if (res >= -BOUND && res <= BOUND &&
                        !dp[mask][res + OFFSET]) {
                        dp[mask][res + OFFSET] = 1;
                        values[mask][valcnt[mask]++] = res;
                    }
                    /* / (x / y) */
                    if (y != 0 && x % y == 0) {
                        res = x / y;
                        if (res >= -BOUND && res <= BOUND &&
                            !dp[mask][res + OFFSET]) {
                            dp[mask][res + OFFSET] = 1;
                            values[mask][valcnt[mask]++] = res;
                        }
                    }
                    /* / (y / x) */
                    if (x != 0 && y % x == 0) {
                        res = y / x;
                        if (res >= -BOUND && res <= BOUND &&
                            !dp[mask][res + OFFSET]) {
                            dp[mask][res + OFFSET] = 1;
                            values[mask][valcnt[mask]++] = res;
                        }
                    }
                }
            }
        }
    }

    /* collect impossible numbers ≤1080 */
    bool reachable[1081];
    memset(reachable, 0, sizeof(reachable));
    for (size_t i = 0; i < valcnt[FULLMASK]; ++i) {
        int val = values[FULLMASK][i];
        if (val >= 1 && val <= 1080)
            reachable[val] = true;
    }
    for (int i = 1; i <= 1080; ++i) {
        if (!reachable[i]) {
            impossible[impossible_cnt++] = i;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        build_tables();
        initialized = true;
    }

    /* if the nth impossible number is among the precomputed ones */
    if (n <= (idx_t)impossible_cnt) {
        return (seq_t)impossible[(size_t)(n - 1)];
    }
    /* otherwise all larger numbers are impossible */
    return (seq_t)(1080 + (seq_t)(n - (idx_t)impossible_cnt));
}
