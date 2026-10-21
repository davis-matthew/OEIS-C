/*
 * A231800  Number of (n+1)X(2+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors equal to one.
 * https://oeis.org/A231800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 binary arrays such that no cell has a
 *            strict majority of its horizontal, vertical, diagonal and
 *            antidiagonal neighbors equal to one
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Returns true iff all cells of cur satisfy the constraint
   with the given neighbors.  cur is the current row,
   up is the row above (if has_above), low is the row below
   (if has_lower).  All rows are 3‑bit patterns (bit i = 1
   if cell i is 1). */
static inline bool row_valid(unsigned int cur,
                             unsigned int up,
                             unsigned int low,
                             bool has_above,
                             bool has_lower)
{
    for (int col = 0; col < 3; ++col) {
        int total = 0;
        int ones  = 0;

        /* horizontal neighbours (left/right) */
        if (col > 0) {
            total++;
            if (cur & (1u << (col - 1))) ones++;
        }
        if (col < 2) {
            total++;
            if (cur & (1u << (col + 1))) ones++;
        }

        /* vertical and diagonal neighbours in the row above */
        if (has_above) {
            /* up */
            total++;
            if (up & (1u << col)) ones++;

            /* up‑left */
            if (col > 0) {
                total++;
                if (up & (1u << (col - 1))) ones++;
            }
            /* up‑right */
            if (col < 2) {
                total++;
                if (up & (1u << (col + 1))) ones++;
            }
        }

        /* vertical and diagonal neighbours in the row below */
        if (has_lower) {
            /* down */
            total++;
            if (low & (1u << col)) ones++;

            /* down‑left */
            if (col > 0) {
                total++;
                if (low & (1u << (col - 1))) ones++;
            }
            /* down‑right */
            if (col < 2) {
                total++;
                if (low & (1u << (col + 1))) ones++;
            }
        }

        /* strict majority forbidden */
        if (ones > total / 2) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t N = n + 1;                 /* number of rows */
    /* dp_prev[p][q] – number of arrays up to the current row
       where the last two rows are patterns p (row i‑1) and q (row i). */
    seq_t dp_prev[8][8] = {0};
    seq_t dp_curr[8][8] = {0};

    /* initialise with all possible first two rows that satisfy
       the constraint for the first row (no upper neighbours). */
    for (int r0 = 0; r0 < 8; ++r0) {
        for (int r1 = 0; r1 < 8; ++r1) {
            if (row_valid((unsigned)r0, 0, (unsigned)r1,
                          /* has_above */ false,
                          /* has_lower */ true))
                dp_prev[r0][r1] += 1;
        }
    }

    /* process remaining rows 2 … N-1 */
    for (idx_t i = 2; i < N; ++i) {
        /* reset current DP */
        for (int p = 0; p < 8; ++p)
            for (int q = 0; q < 8; ++q)
                dp_curr[p][q] = 0;

        /* transition: add new row s as lower neighbour of q */
        for (int p = 0; p < 8; ++p) {
            for (int q = 0; q < 8; ++q) {
                seq_t cnt = dp_prev[p][q];
                if (!cnt) continue;
                for (int s = 0; s < 8; ++s) {
                    if (row_valid((unsigned)q, (unsigned)p, (unsigned)s,
                                  /* has_above */ true,
                                  /* has_lower */ true))
                        dp_curr[q][s] += cnt;
                }
            }
        }

        /* prepare for next iteration */
        for (int p = 0; p < 8; ++p)
            for (int q = 0; q < 8; ++q)
                dp_prev[p][q] = dp_curr[p][q];
    }

    /* final sum – validate the last row (no lower neighbours) */
    seq_t result = 0;
    for (int p = 0; p < 8; ++p) {
        for (int q = 0; q < 8; ++q) {
            if (!dp_prev[p][q]) continue;
            if (row_valid((unsigned)q, (unsigned)p, 0,
                          /* has_above */ true,
                          /* has_lower */ false))
                result += dp_prev[p][q];
        }
    }
    return result;
}
