/*
 * A032841  Numbers whose base-3 representation Sum_{i=0..m} d(i)*3^i has d(m)
 *          < d(m-1) > d(m-2) < ...
 * https://oeis.org/A032841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032841 – Numbers whose base‑3 digits alternate
 * with inequalities starting: d(m) < d(m-1) > d(m-2) < … .
 * The n-th term a(n) (n≥1) is returned in the integer type seq_t.
 *
 * The algorithm counts how many admissible base‑3 strings exist for
 * each length, finds the appropriate length for the requested
 * n, then constructs the corresponding base‑3 number digit by
 * digit using the counts stored in a DP table.  The algorithm
 * runs in O(L) per call, where L is the number of base‑3 digits
 * of a(n) (≤ 30 for n up to 10⁴).  All arithmetic uses only
 * seq_t operations, so the code works for 8, 16, 32, 64 or 128 bit
 * unsigned integers as well as arbitrary–precision types compiled
 * as C++17.
 */

#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  DP table:  dp[rem][prev][p]  – number of completions
 *
 *      rem     : number of digits still to place after the current one
 *      prev    : value of the current (most recent) digit (0,1,2)
 *      p       : 0 → next digit must be greater than 'prev'
 *               1 → next digit must be smaller than 'prev'
 *
 *  The table is filled once up to a generous maximum of 70 digits
 *  (more than enough for n up to 10⁴).  It is indexed only by
 *  small integers, so all values fit safely in seq_t.
 * ------------------------------------------------------------------ */
static bool dp_init = false;
static seq_t dp[70][3][2];

static void init_dp(void)
{
    /* base case: no digits left → exactly one completion */
    for (int d = 0; d < 3; ++d)
        for (int p = 0; p < 2; ++p)
            dp[0][d][p] = 1;

    /* build up to 70 digits */
    for (int rem = 1; rem < 70; ++rem) {
        for (int prev = 0; prev < 3; ++prev) {
            for (int p = 0; p < 2; ++p) {
                seq_t s = 0;
                for (int nxt = 0; nxt < 3; ++nxt) {
                    if ((p == 0 && nxt > prev) || (p == 1 && nxt < prev))
                        s += dp[rem - 1][nxt][p ^ 1];
                }
                dp[rem][prev][p] = s;
            }
        }
    }
    dp_init = true;
}

/* ------------------------------------------------------------------ */
/*  Main function:  a(idx_t n)  – returns the n-th element
 *  (n ≥ 1).  The implementation searches for the appropriate length
 *  and constructs the number digit by digit using the DP counts.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!dp_init) init_dp();

    /* 1. Find the length L of the n-th number. */
    seq_t cumulative = 0;
    idx_t  L = 1;
    seq_t countLen;

    while (true) {
        if (L == 1)
            countLen = 2;                          /* {1,2} */
        else
            countLen = dp[L - 1][1][0] + dp[L - 1][2][0];

        if ((seq_t)n <= cumulative + countLen)
            break;
        cumulative += countLen;
        ++L;
    }

    /* 2. k is the (1‑based) index within all admissible strings of length L. */
    seq_t k = (seq_t)n - cumulative;

    /* 3. Construct the number. */
    seq_t value = 0;          /* accumulated value in base 3 */
    int    prev  = 0;         /* last chosen digit (unused for first pos) */

    for (int pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;   /* most significant digit ≠ 0 */
        for (int d = start; d <= 2; ++d) {
            seq_t ways = 0;

            if (pos == 0) {                                 /* first digit */
                ways = dp[L - 1][d][0];                    /* next must be > */
            } else {
                /* inequality for the current digit depends on position:
                   odd pos → must be greater than previous,
                   even pos → must be smaller.           */
                bool greater = (pos % 2 == 1);              /* current comp */
                if (greater) {
                    if (!(d > prev)) continue;             /* need d > prev */
                } else {
                    if (!(d < prev)) continue;             /* need d < prev */
                }

                int remaining = L - 1 - pos;                /* digits after this */
                ways = dp[remaining][d][greater];          /* parity for next */
            }

            if (k > ways) {
                k -= ways;
                continue;
            } else {
                value = value * 3 + d;
                prev  = d;
                break;  /* digit chosen */
            }
        }
    }

    return value;
}
