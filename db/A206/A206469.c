/*
 * A206469  Number of (n+1)X6 0..2 arrays with every 2X2 subblock having zero
 *          permanent.
 * https://oeis.org/A206469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206469.  This is a DP over rows with
   states compressed to zero/non‑zero patterns (6 bits).
   The DP counts (n+1)×6 arrays over {0,1,2} whose every
   2×2 subblock has permanent 0 (i.e. each opposite corner
   pair contains a zero). */

#include <stdbool.h>
#include <stddef.h>

#define SZ 64               /* number of 6‑bit zero/non‑zero masks */

seq_t a(idx_t n)
{
    /* number of rows in the array is n+1 (n ≥ 1) */
    idx_t rows = n + 1;     /* rows ≥ 2 */
    /* weight[m] = 2^(popcount(m)) – number of actual patterns
       that have the same zero/non‑zero mask */
    static seq_t weight[SZ];
    static bool weight_init = false;
    if (!weight_init) {
        for (int mask = 0; mask < SZ; ++mask) {
            int pc = 0;
            unsigned u = mask;
            while (u) { pc += u & 1; u >>= 1; }
            weight[mask] = ((seq_t)1 << pc);
        }
        weight_init = true;
    }

    /* comp[prev][cur] == true  if the two masks can be consecutive rows */
    static bool comp[SZ][SZ];
    static bool comp_init = false;
    if (!comp_init) {
        for (int m1 = 0; m1 < SZ; ++m1) {
            for (int m2 = 0; m2 < SZ; ++m2) {
                bool ok = ((m1 & (m2 >> 1)) == 0) &&
                          (((m1 >> 1) & m2) == 0);
                comp[m1][m2] = ok;
            }
        }
        comp_init = true;
    }

    /* DP arrays: dp[m] – number of ways for the current number of rows
       ending with mask m.  Use two arrays to iterate over rows. */
    seq_t dp[SZ], ndp[SZ];
    for (int m = 0; m < SZ; ++m) dp[m] = weight[m]; /* first row */

    /* Process remaining (rows-1) transitions */
    for (idx_t step = 2; step <= rows; ++step) {
        for (int m2 = 0; m2 < SZ; ++m2) {
            seq_t sum = 0;
            for (int m1 = 0; m1 < SZ; ++m1)
                if (comp[m1][m2])
                    sum += dp[m1];
            ndp[m2] = sum * weight[m2];
        }
        for (int m = 0; m < SZ; ++m) dp[m] = ndp[m];
    }

    /* Sum over all ending masks to get total number of arrays */
    seq_t result = 0;
    for (int m = 0; m < SZ; ++m)
        result += dp[m];
    return result;
}
