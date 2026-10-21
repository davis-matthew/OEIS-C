/*
 * A231527  Number of 5Xn 0..1 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A231527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of 5×n binary arrays in which no * element is
 *            less than the strict majority of its horizontal, * diagonal, and
 *            anti‑diagonal neighbours. * The value is obtained by dynamic
 *            programming over column masks
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   check_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/check_col.h"

/* Masks for the 5 rows (bit 0 = row 0, bit 4 = row 4).  */
#define MASKS 32

/* Helper tables, built once. */
static bool  valid_left_only[MASKS][MASKS];   /* column has left absent, right present   */
static bool  valid_right_only[MASKS][MASKS];  /* column has left present, right absent    */
static bool  valid_no_neigh[MASKS];           /* column with no neighbours               */
static uint32_t next_mask[MASKS][MASKS];      /* for each (prev,curr) the mask of allowed next */
static bool tables_ready = false;

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Initialise static tables once. */
static void
init_tables(void)
{
    if (tables_ready) return;

    for (int L = 0; L < MASKS; ++L) {
        for (int M = 0; M < MASKS; ++M) {
            for (int R = 0; R < MASKS; ++R) {
                if (check_col(M, L, R, true, true))
                    next_mask[L][M] |= (1u << R);
            }
        }
    }

    for (int M = 0; M < MASKS; ++M)
        for (int R = 0; R < MASKS; ++R)
            valid_left_only[M][R] = check_col(M, 0, R, false, true);

    for (int L = 0; L < MASKS; ++L)
        for (int M = 0; M < MASKS; ++M)
            valid_right_only[L][M] = check_col(M, L, 0, true, false);

    for (int M = 0; M < MASKS; ++M)
        valid_no_neigh[M] = check_col(M, 0, 0, false, false);

    tables_ready = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;          /* undefined, but harmless */

    if (n == 1) {                  /* single column */
        seq_t ans = 0;
        for (int M = 0; M < MASKS; ++M)
            if (valid_no_neigh[M]) ans += (seq_t)1;
        return ans;
    }

    /* Dynamic programming over column masks.  */
    seq_t dp[MASKS * MASKS];
    seq_t newdp[MASKS * MASKS];
    for (int i = 0; i < MASKS * MASKS; ++i) dp[i] = newdp[i] = 0;

    /* Initial state: first column (no left neighbour), second column (present). */
    for (int a = 0; a < MASKS; ++a) {
        for (int b = 0; b < MASKS; ++b) {
            if (valid_left_only[a][b])
                dp[a * MASKS + b] = (seq_t)1;
        }
    }

    if (n == 2) {                  /* end-of-array checks for second column */
        seq_t ans = 0;
        for (int a = 0; a < MASKS; ++a) {
            for (int b = 0; b < MASKS; ++b) {
                seq_t val = dp[a * MASKS + b];
                if (!val) continue;
                if (valid_right_only[a][b]) ans += val;
            }
        }
        return ans;
    }

    /* Transition for columns 2 .. n-1 (inclusive). */
    idx_t steps = n - 2;
    while (steps--) {
        for (int i = 0; i < MASKS * MASKS; ++i) newdp[i] = 0;
        for (int prev = 0; prev < MASKS; ++prev) {
            for (int curr = 0; curr < MASKS; ++curr) {
                seq_t val = dp[prev * MASKS + curr];
                if (!val) continue;
                uint32_t mask = next_mask[prev][curr];
                while (mask) {
                    int next = __builtin_ctz(mask);
                    newdp[curr * MASKS + next] += val;
                    mask &= mask - 1;
                }
            }
        }
        for (int i = 0; i < MASKS * MASKS; ++i) dp[i] = newdp[i];
    }

    /* Final column check (right neighbour missing). */
    seq_t ans = 0;
    for (int a = 0; a < MASKS; ++a) {
        for (int b = 0; b < MASKS; ++b) {
            seq_t val = dp[a * MASKS + b];
            if (!val) continue;
            if (valid_right_only[a][b]) ans += val;
        }
    }

    return ans;
}
