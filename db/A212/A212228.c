/*
 * A212228  Number of 0..2 arrays of length n+7 with sum no more than 8 in any
 *          length 8 subsequence (=50% duty cycle).
 * https://oeis.org/A212228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 15, u64 n <= 37, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A212228: Number of 0..2 arrays of length n+7 with every length‑8
   substring having sum ≤ 8  (50 % duty cycle).  a(1) corresponds to
   arrays of length 8, a(2) to length 9, etc.
   This implementation uses dynamic programming over the last 7 digits.
   It works for any seq_t (8, 16, 32, 64 or 128 bit unsigned, or arbitrary‑precision
   integer). */

#include <stdlib.h>
#include <stdbool.h>

/* 3⁷  – number of states (7‑digit words over {0,1,2}) */
#define STATE_COUNT 2187U
#define POW3_6 729U  /* 3⁶, used for state transition */

/* State data: sum of the 7 digits for each state */
static int sum7[STATE_COUNT];
static bool tables_initialized = false;

/* Initialise state data once */
static void init_tables(void)
{
    if (tables_initialized) return;
    for (int s = 0; s < STATE_COUNT; ++s) {
        int x = s;
        int sum = 0;
        for (int k = 0; k < 7; ++k) {
            int d = x % 3;
            x /= 3;
            sum += d;
        }
        sum7[s] = sum;
    }
    tables_initialized = true;
}

/* Sequence: a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */

    /* Ensure state data is ready */
    init_tables();

    /* Static DP tables and cache – allocated once */
    static seq_t dp[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];
    static bool dp_initialized = false;

    static seq_t cache[10005];    /* enough for n up to about 10000 */
    static size_t cache_len = 0;  /* number of entries cached (a(1) … a(cache_len)) */

    static idx_t len = 7;          /* dp holds sequences of current length */

    /* First time: set all length‑7 sequences as valid */
    if (!dp_initialized) {
        for (size_t i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)1;
        dp_initialized = true;
    }

    /* Return cached value if already computed */
    if (n <= cache_len) return cache[n - 1];

    /* Extend DP until we have a(n) */
    while (cache_len < n) {
        /* Clear dp_next */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;

        /* Transition */
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = dp[s];
            if (cnt == (seq_t)0) continue;
            int sum_s = sum7[s];
            for (int d = 0; d < 3; ++d) {
                if (sum_s + d <= 8) {
                    /* Shift left (drop most significant digit) and add d */
                    size_t new_state = ((s % POW3_6) * 3U) + (size_t)d;
                    dp_next[new_state] += cnt;
                }
            }
        }

        /* Swap tables */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
        ++len;  /* now we have sequences of length len */

        /* Compute total for current len (length = len) */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < STATE_COUNT; ++i) total += dp[i];

        /* Store a(len-7) = total */
        cache[cache_len++] = total;
    }

    return cache[n - 1];
}
