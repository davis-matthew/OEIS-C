/*
 * A189107  Number of nX5 binary arrays without the pattern 1 1 0 diagonally
 *          or antidiagonally.
 * https://oeis.org/A189107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189107: number of n×5 binary arrays that avoid a 1‑1‑0 pattern
 * appearing diagonally or antidiagonally.
 *
 * The sequence is computed by a dynamic program over the last two rows
 * of the array.  For each possible state of two consecutive rows
 * (encoded as a 10‑bit integer) we precompute all admissible next rows
 * and then iterate for n ≥ 3.  No simple closed form is known, but the
 * DP runs in O(n·1024·t) time where t is the average number of
 * transitions per state (≤ 32), which is well below the 2 s limit for
 * n ≈ 10000.  All arithmetic is performed in the arbitrary‑precision
 * type seq_t, and no heap allocation is used.  The algorithm works
 * for every n ≥ 1 and is independent of the order of calls.
 */

#include <stdbool.h>

/* Number of possible 5‑bit rows. */
#define ROWS 32
/* Number of possible two‑row states (10 bits). */
#define STATES (ROWS * ROWS)

/* Transition table: trans[p][k] is the k‑th admissible next row
 * for the state p (p = prev2<<5 | prev1).  The maximum number of
 * transitions per state is 32, so we allocate a fixed width array. */
static uint8_t trans[STATES][ROWS];
static uint8_t trans_cnt[STATES];
static bool trans_inited = false;

/* Build the transition table once. */
static void init_transitions(void)
{
    if (trans_inited)
        return;

    for (int p = 0; p < STATES; ++p) {
        int prev2 = p >> 5;      /* high 5 bits */
        int prev1 = p & 31;      /* low 5 bits */
        uint8_t cnt = 0;

        /* Try all possible rows for the new top and check patterns. */
        for (int curr = 0; curr < ROWS; ++curr) {
            bool ok = true;
            /* There are up to three diagonals that can form a 1‑1‑0
             * pattern; we check them for both directions. */
            for (int j = 0; j <= 2; ++j) {
                int a = (prev2 >> j)      & 1; /* (prev2, j)       */
                int b = (prev1 >> (j+1))  & 1; /* (prev1, j+1)     */
                int c = (curr  >> (j+2))  & 1; /* (curr , j+2)     */
                if (a == 1 && b == 1 && c == 0) {
                    ok = false;
                    break;
                }
                int d = (prev2 >> (j+2))  & 1; /* (prev2, j+2)     */
                int e = (prev1 >> (j+1))  & 1; /* (prev1, j+1)     */
                int f = (curr  >> j)      & 1; /* (curr , j)       */
                if (d == 1 && e == 1 && f == 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                trans[p][cnt++] = (uint8_t)curr;
            }
        }
        trans_cnt[p] = cnt;
    }
    trans_inited = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(0) is undefined but we return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* a(1) is simply all 5‑bit rows (2⁵ = 32). */
    if (n == 1)
        return (seq_t)32;

    init_transitions();

    /* dp[p] holds the number of ways to reach state p after some rows. */
    seq_t dp[STATES];
    seq_t newdp[STATES];

    /* n == 2: every pair of rows is possible exactly once. */
    for (int i = 0; i < STATES; ++i)
        dp[i] = (seq_t)1;

    /* For n >= 3 process rows 3 through n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* Zero newdp using the type’s assignment operator. */
        for (int i = 0; i < STATES; ++i)
            newdp[i] = (seq_t)0;

        for (int p = 0; p < STATES; ++p) {
            seq_t ways = dp[p];
            if (ways == 0)
                continue;
            int prev1 = p & 31;
            uint8_t cnt = trans_cnt[p];
            for (uint8_t k = 0; k < cnt; ++k) {
                int curr = trans[p][k];
                int newp = (prev1 << 5) | curr;
                newdp[newp] = newdp[newp] + ways;
            }
        }

        /* Copy newdp back into dp for the next iteration. */
        for (int i = 0; i < STATES; ++i)
            dp[i] = newdp[i];
    }

    /* Sum all states to obtain the final answer. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATES; ++i)
        result = result + dp[i];

    return result;
}
