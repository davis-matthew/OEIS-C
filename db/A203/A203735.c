/*
 * A203735  Number of (n+1)X3 0..3 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A203735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 13, u64 n <= 40, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203735:
   Count of (n+1)×3 arrays with entries in {0,1,2,3} such that the
   pair sums b(i,j)=a(i,j)+a(i,j‑1) and c(i,j)=a(i,j)+a(i‑1,j) are
   nondecreasing in the column and row directions respectively.
   Computation is performed by dynamic programming over rows; see
   algorithm description below. */
#include <stdbool.h>           /* for bool, true, false */

#define NB 64                  /* number of possible rows = 4^3 */

static unsigned char ok[NB][NB]; /* transition table, built once */
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Build transition table lazily. */
    if (!initialized) {
        for (unsigned p = 0; p < NB; ++p) {
            unsigned p0 = (p >> 4) & 3;
            unsigned p1 = (p >> 2) & 3;
            unsigned p2 =  p        & 3;
            for (unsigned q = 0; q < NB; ++q) {
                unsigned q0 = (q >> 4) & 3;
                unsigned q1 = (q >> 2) & 3;
                unsigned q2 =  q        & 3;
                /* Nondecreasing pair‑sums constraints. */
                if (p1 + p0 <= q1 + q0 && /* b(i‑1,1) ≤ b(i,1) */
                    p2 + p1 <= q2 + q1 && /* b(i‑1,2) ≤ b(i,2) */
                    q0 + p0 <= q1 + p1 && /* c(i,0)   ≤ c(i,1) */
                    q1 + p1 <= q2 + p2)    /* c(i,1)   ≤ c(i,2) */
                    ok[p][q] = 1;
            }
        }
        initialized = true;
    }

    /* DP over rows.  dp[r] = number of ways to finish with last row r. */
    seq_t dp[NB];
    seq_t ndp[NB];

    for (unsigned r = 0; r < NB; ++r) dp[r] = 1;       /* any first row */

    /* Perform n transitions (n+1 rows in total). */
    for (idx_t step = 1; step <= n; ++step) {
        for (unsigned r = 0; r < NB; ++r) ndp[r] = 0;

        for (unsigned prev = 0; prev < NB; ++prev) {
            seq_t val = dp[prev];
            if (val == 0) continue;
            for (unsigned curr = 0; curr < NB; ++curr) {
                if (ok[prev][curr]) ndp[curr] += val;
            }
        }
        for (unsigned r = 0; r < NB; ++r) dp[r] = ndp[r];
    }

    /* Sum over all possible last rows. */
    seq_t result = 0;
    for (unsigned r = 0; r < NB; ++r) result += dp[r];
    return result;
}
