/*
 * A164139  Number of binary strings of length n with equal numbers of 000 and
 *          011 substrings.
 * https://oeis.org/A164139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of binary strings of length n with an equal
 *            number of substrings 000 and 011. The implementation uses
 *            dynamic programming over the last two bits of the string
 *            together with the current difference D = (#000) – (#011). States
 *            0–3 represent the last two bits 00,01,10,11. At each s...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Transition tables: next state and change in difference for each
   current state and appended bit.  */
static const int next_state[4][2] = {
    {0, 1},   /* 00 → 000 (+1) / 001 (0)  */
    {2, 3},   /* 01 → 010 (0)  / 011 (–1) */
    {0, 1},   /* 10 → 100 (0)  / 101 (0)  */
    {2, 3}    /* 11 → 110 (0)  / 111 (0)  */
};

static const int delta[4][2] = {
    { +1,  0 },   /* from 00, add 0 → +1, add 1 → 0   */
    {  0, -1 },   /* from 01, add 0 → 0,  add 1 → –1 */
    {  0,  0 },   /* from 10, add 0 → 0,  add 1 → 0  */
    {  0,  0 }    /* from 11, add 0 → 0,  add 1 → 0  */
};

/* Static state for the DP and memoisation */
static bool initialized = false;
static idx_t cur_len = 0;            /* longest length processed so far   */
static size_t cur_range = 0;         /* size of DP arrays for cur_len     */
static seq_t *dp[4] = {0, 0, 0, 0};  /* DP arrays for current length      */

static idx_t cache_len = 0;
static seq_t *cache = NULL;

/* Helper: ensure cache array is large enough for at least n+1 entries */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)calloc(new_len, sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_len = new_len;
}

/* Initialise the DP tables for length 2 (base case). */
static void init_dp(void)
{
    cur_len   = 2;
    cur_range = 1;      /* only difference 0 is possible for length 2 */

    for (int s = 0; s < 4; ++s) {
        dp[s] = (seq_t *)calloc(cur_range, sizeof(seq_t));
        dp[s][0] = (seq_t)1;  /* exactly one string ends with each 2‑bit pattern */
    }
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* First call initialisation */
    if (!initialized) {
        /* base values for n = 0,1,2 */
        ensure_cache(2);
        cache[0] = (seq_t)1;   /* empty string */
        cache[1] = (seq_t)2;   /* "0", "1" */
        cache[2] = (seq_t)4;   /* "00", "01", "10", "11" */

        init_dp();
        cache[2] = (seq_t)4;   /* value already present after init_dp */

        initialized = true;
    }

    /* Return cached value if already available */
    if (n < cache_len)
        return cache[n];

    /* Extend the DP until we reach length n */
    while (cur_len < n) {
        idx_t step = cur_len + 1;
        size_t new_range = 2 * (step - 2) + 1;   /* range for new length */

        /* Allocate new DP arrays for the next length */
        seq_t *new_dp[4];
        for (int s = 0; s < 4; ++s)
            new_dp[s] = (seq_t *)calloc(new_range, sizeof(seq_t));

        /* Perform transitions from current state to new state */
        for (int s = 0; s < 4; ++s) {
            for (size_t i = 0; i < cur_range; ++i) {
                seq_t val = dp[s][i];
                if (!val)
                    continue;

                /* Append bit 0 */
                int ns0 = next_state[s][0];
                int d0  = delta[s][0];
                size_t ni0 = i + 1 + d0;              /* shift by +1 for offset change */
                new_dp[ns0][ni0] += val;

                /* Append bit 1 */
                int ns1 = next_state[s][1];
                int d1  = delta[s][1];
                size_t ni1 = i + 1 + d1;
                new_dp[ns1][ni1] += val;
            }
        }

        /* Free old DP arrays and adopt the new ones */
        for (int s = 0; s < 4; ++s) {
            free(dp[s]);
            dp[s] = new_dp[s];
        }

        /* Update DP metadata */
        cur_len   = step;
        cur_range = new_range;

        /* Compute the answer for current length: sum of counts with D = 0 */
        size_t offset = cur_range / 2;               /* index of difference 0 */
        seq_t result  = dp[0][offset] + dp[1][offset] +
                        dp[2][offset] + dp[3][offset];

        /* Cache the result */
        ensure_cache(cur_len);
        cache[cur_len] = result;
    }

    return cache[n];
}
