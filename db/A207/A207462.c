/*
 * A207462  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207462:
   Number of n × 3 0/1 arrays avoiding horizontal patterns 000 and 011
   and vertical patterns 010 and 101 among consecutive rows.
   Recurrence (n ≥ 3):
     a(n) = Σ_{p,q,r allowed} a_{p,q}
   where a_{p,q} is the number of arrays of length n‑1 ending with rows
   p (previous‑previous) and q (previous); transition p,q→q,r is allowed
   iff for every column the triple (p_c,q_c,r_c) is not 010 nor 101.
   Initial values: a(1) = 6, a(2) = 36. */

#include <stdint.h>
#include <stdbool.h>

/* Three‑bit rows that avoid the forbidden horizontal 000 and 011. */
static const int ROWS[6] = {1, 2, 4, 5, 6, 7};

/* For each pair (prevprev,prev), a bitmask of allowed next rows. */
static unsigned char ALLOWED_NEXT[6][6];
static bool ALLOWED_NEXT_INIT = false;

static void init_allowed_next(void)
{
    for (int p = 0; p < 6; ++p) {
        for (int q = 0; q < 6; ++q) {
            unsigned char mask = 0;
            for (int r = 0; r < 6; ++r) {
                bool ok = true;
                for (int c = 0; c < 3; ++c) {
                    int b1 = (ROWS[p] >> c) & 1;
                    int b2 = (ROWS[q] >> c) & 1;
                    int b3 = (ROWS[r] >> c) & 1;
                    if ((b1 == 0 && b2 == 1 && b3 == 0) ||
                        (b1 == 1 && b2 == 0 && b3 == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) mask |= (1u << r);
            }
            ALLOWED_NEXT[p][q] = mask;
        }
    }
    ALLOWED_NEXT_INIT = true;
}

seq_t a(idx_t n)
{
    if (!ALLOWED_NEXT_INIT) init_allowed_next();

    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)36;

    /* dp[p][q] = number of arrays ending with rows p (prevprev) and q (prev). */
    seq_t dp[6][6] = {{0}};
    seq_t ndp[6][6] = {{0}};

    /* Base for length 2: all 6×6 pairs are valid. */
    for (int p = 0; p < 6; ++p) {
        for (int q = 0; q < 6; ++q) {
            dp[p][q] = (seq_t)1;
        }
    }

    for (idx_t row = 3; row <= n; ++row) {
        /* zero ndp */
        for (int p = 0; p < 6; ++p)
            for (int q = 0; q < 6; ++q)
                ndp[p][q] = (seq_t)0;

        for (int p = 0; p < 6; ++p) {
            for (int q = 0; q < 6; ++q) {
                seq_t cur = dp[p][q];
                if (cur == (seq_t)0) continue;
                unsigned char mask = ALLOWED_NEXT[p][q];
                if (mask == 0) continue;
                for (int r = 0; r < 6; ++r) {
                    if (mask & (1u << r)) {
                        ndp[q][r] = ndp[q][r] + cur;
                    }
                }
            }
        }
        /* copy ndp -> dp for next iteration */
        for (int p = 0; p < 6; ++p)
            for (int q = 0; q < 6; ++q)
                dp[p][q] = ndp[p][q];
    }

    /* sum all endings to get total count. */
    seq_t total = (seq_t)0;
    for (int p = 0; p < 6; ++p)
        for (int q = 0; q < 6; ++q)
            total = total + dp[p][q];

    return total;
}
