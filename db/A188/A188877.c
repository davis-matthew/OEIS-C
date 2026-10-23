/*
 * A188877  Number of 5Xn binary arrays without the pattern 0 0 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A188877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: transfer‐matrix dynamic programming.
   State = pair of consecutive columns (10 bits).  
   Transition: add a new column, rejecting any transition that
   creates a horizontal 000 or an antidiagonal 000.  
   The algorithm runs in O(n * 2^10 * 5) time and is memoised. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define COL_BITS 5
#define COL_MASK ((1U << COL_BITS) - 1)
#define STATE_BITS 10
#define STATE_SIZE (1U << STATE_BITS)

/* Cached DP tables */
static seq_t *dp_cur   = NULL;
static seq_t *dp_next  = NULL;
static idx_t  max_done = 1;   /* dp_cur is valid for columns up to max_done */

/* Allocate DP tables once (C++ new, C malloc) */
static void init_dp(void)
{
    if (dp_cur != NULL)
        return;

#ifdef __cplusplus
    dp_cur  = new seq_t[STATE_SIZE];
    dp_next = new seq_t[STATE_SIZE];
#else
    dp_cur  = calloc(STATE_SIZE, sizeof(seq_t));
    dp_next = calloc(STATE_SIZE, sizeof(seq_t));
#endif
    if (!dp_cur || !dp_next)
        abort();

    /* For n = 2 the number of arrays for each pair of columns is 1 */
    for (uint32_t s = 0; s < STATE_SIZE; ++s)
        dp_cur[s] = (seq_t)1;

    max_done = 2;   /* dp_cur holds results for 2 columns */
}

/* Transfer matrix step for one new column */
static inline bool bad_transition(uint32_t c2, uint32_t c1, uint32_t newc)
{
    /* Horizontal 000: a row with zeros in all three columns is forbidden */
    if (((~c2) & (~c1) & (~newc) & COL_MASK) != 0)
        return true;

    /* Antidiagonal 000: check rows 0..2 (rows r, r+1, r+2) */
    for (int r = 0; r <= 2; ++r) {
        if (((c2 >> r)     & 1) == 0 &&
            ((c1 >> (r+1)) & 1) == 0 &&
            ((newc >> (r+2)) & 1) == 0)
            return true;
    }
    return false;
}

/* Public function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */

    if (n == 1)
        return (seq_t)32;         /* 2^5 */
    if (n == 2)
        return (seq_t)1024;       /* 2^10 */

    init_dp();

    /* Extend DP up to column n */
    for (idx_t col = max_done + 1; col <= n; ++col) {
        /* clear next table */
        for (uint32_t i = 0; i < STATE_SIZE; ++i)
            dp_next[i] = (seq_t)0;

        for (uint32_t s = 0; s < STATE_SIZE; ++s) {
            seq_t val = dp_cur[s];
            if (!val)
                continue;

            uint32_t c2 = (s >> COL_BITS) & COL_MASK; /* column j-2 */
            uint32_t c1 =  s            & COL_MASK;    /* column j-1 */

            for (uint32_t newc = 0; newc < 32; ++newc) {
                if (bad_transition(c2, c1, newc))
                    continue;
                uint32_t ns = (c1 << COL_BITS) | newc;    /* new state (j-1,j) */
                dp_next[ns] += val;
            }
        }
        /* swap current and next for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        ++max_done;
    }

    /* Sum all states after processing n columns */
    seq_t ans = (seq_t)0;
    for (uint32_t s = 0; s < STATE_SIZE; ++s)
        ans += dp_cur[s];
    return ans;
}
