/*
 * A183373  Number of nX6 binary arrays with no element equal to the mod 3 sum
 *          of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays such that for every element
 *            the value is not equal to the sum (mod 3) of its diagonal and
 *            antidiagonal neighbours
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Fixed dimensions -------------------------------------------------- */
#define ROWS   64                  /* 2^6 possible row masks            */
#define STATES (ROWS*ROWS)          /* pair (prev,cur) states           */

/* Pre‑computed transitions ------------------------------------------ */
static uint8_t  nextlist[STATES][ROWS];   /* for each (prev,cur) the list of next rows */
static uint8_t  nextcnt [STATES];         /* number of entries in nextlist   */
static bool     nextlist_inited = false;  /* init flag                       */

/* Check whether a row 'cur' is valid with neighbours 'prev' and 'next' */
static inline bool row_valid(uint8_t cur, uint8_t prev, uint8_t next)
{
    for (int j = 0; j < 6; ++j) {
        int sum = 0;
        if (j > 0)          sum += (prev >> (j-1)) & 1;  /* diag   prev,j-1   */
        if (j < 5)          sum += (prev >> (j+1)) & 1;  /* antidiag prev,j+1 */
        if (j > 0)          sum += (next >> (j-1)) & 1;  /* diag   next,j-1   */
        if (j < 5)          sum += (next >> (j+1)) & 1;  /* antidiag next,j+1 */
        sum %= 3;
        int bit = (cur >> j) & 1;
        if (bit == sum)
            return false;                          /* forbidden value  */
    }
    return true;
}

/* Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise transition tables once --------------------------------- */
    if (!nextlist_inited) {
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int cur = 0; cur < ROWS; ++cur) {
                int idx  = prev * ROWS + cur;
                int count = 0;
                for (int next = 0; next < ROWS; ++next) {
                    if (row_valid((uint8_t)cur, (uint8_t)prev, (uint8_t)next))
                        nextlist[idx][count++] = (uint8_t)next;
                }
                nextcnt[idx] = (uint8_t)count;
            }
        }
        nextlist_inited = true;
    }

    /* DP arrays for two consecutive layers -------------------------------- */
    static seq_t dp[STATES];
    static seq_t nextdp[STATES];

    /* clear dp arrays (needed on first call or if n has changed) ------------ */
    for (int i = 0; i < STATES; ++i) {
        dp[i]     = 0;
        nextdp[i] = 0;
    }

    /* Initial state: imaginary top row = 0, first real row = mask          */
    for (int mask = 0; mask < ROWS; ++mask) {
        dp[mask] = 1;                  /* state (prev=0, cur=mask) */
    }

    /* Transition through the remaining rows (n-1) --------------------------- */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < STATES; ++i)
            nextdp[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp[s];
            if (!val) continue;
            int cnt  = nextcnt[s];
            uint8_t *list = nextlist[s];
            for (int t = 0; t < cnt; ++t) {
                int ns = (s & 63) * ROWS + list[t];   /* (cur,next) state */
                nextdp[ns] += val;
            }
        }
        /* swap dp and nextdp for next iteration -------------------------- */
        for (int i = 0; i < STATES; ++i)
            dp[i] = nextdp[i];
    }

    /* Final step: add imaginary bottom row = 0, check validity of last row */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s) {
        seq_t val = dp[s];
        if (!val) continue;
        /* list contains all next rows that keep 'cur' valid with 'prev'.  */
        for (int t = 0; t < nextcnt[s]; ++t) {
            if (nextlist[s][t] == 0) {            /* bottom row 0 valid?   */
                result += val;
                break;
            }
        }
    }
    return result;
}
