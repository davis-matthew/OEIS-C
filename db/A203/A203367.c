/*
 * A203367  Number of nX4 0..2 arrays with every 1 immediately preceded by 0
 *          to the left or above, and every 2 immediately preceded by both a 1
 *          and a 0.
 * https://oeis.org/A203367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts the number of n×4 arrays with entries 0..2
   satisfying:
   • each 1 has a 0 immediately to its left or above,
   • each 2 has both a 1 and a 0 immediately to its left or above.
   It uses dynamic programming over the 3⁴ = 81 possible row patterns,
   precomputing a table of compatible row pairs.  */

#include <stddef.h>

seq_t a(idx_t n)
{
    enum {S = 81};                     /* 3⁴ row patterns */
    static int digit[S][4];
    static seq_t first_row[S];
    static unsigned char compat[S][S];

    /* ------------------------------------------------------------------ */
    /* One‑time construction of helper tables. */
    static int initialized = 0;
    if (!initialized) {
        /* Convert mask number into base‑3 digits (row contents). */
        for (int mask = 0; mask < S; ++mask) {
            int x = mask;
            for (int j = 3; j >= 0; --j) {
                digit[mask][j] = x % 3;
                x /= 3;
            }
        }

        /* Determine which row patterns may appear as the first row. */
        for (int mask = 0; mask < S; ++mask) {
            int ok = 1;
            for (int j = 0; j < 4 && ok; ++j) {
                int v = digit[mask][j];
                if (v == 2)                   ok = 0;
                else if (v == 1) {
                    if (j == 0 || digit[mask][j - 1] != 0)
                        ok = 0;
                }
            }
            first_row[mask] = ok ? (seq_t)1 : (seq_t)0;
        }

        /* Precompute compatibility of two consecutive rows. */
        for (int prev = 0; prev < S; ++prev) {
            for (int curr = 0; curr < S; ++curr) {
                int ok = 1;
                for (int j = 0; j < 4 && ok; ++j) {
                    int v = digit[curr][j];
                    int left  = (j > 0) ? digit[curr][j - 1] : -1;
                    int above = digit[prev][j];

                    if (v == 1) {
                        if (left != 0 && above != 0)  ok = 0;
                    } else if (v == 2) {
                        int has0 = 0, has1 = 0;
                        if (left >= 0) {
                            if (left == 0) has0 = 1;
                            if (left == 1) has1 = 1;
                        }
                        if (above == 0) has0 = 1;
                        if (above == 1) has1 = 1;
                        if (!(has0 && has1))           ok = 0;
                    }
                }
                compat[prev][curr] = (unsigned char)ok;
            }
        }
        initialized = 1;
    }

    /* ------------------------------------------------------------------ */
    /* Dynamic programming over rows. */
    seq_t dp_prev[S];
    seq_t dp_curr[S];
    int i;

    for (i = 0; i < S; ++i) dp_prev[i] = first_row[i];

    if (n == 1) {
        seq_t total = 0;
        for (i = 0; i < S; ++i) total += dp_prev[i];
        return total;
    }

    for (idx_t row = 1; row < n; ++row) {
        for (int curr = 0; curr < S; ++curr) {
            seq_t sum = 0;
            for (int prev = 0; prev < S; ++prev)
                if (compat[prev][curr])
                    sum += dp_prev[prev];
            dp_curr[curr] = sum;
        }
        for (i = 0; i < S; ++i) {
            dp_prev[i] = dp_curr[i];
            dp_curr[i] = 0;
        }
    }

    seq_t total = 0;
    for (i = 0; i < S; ++i) total += dp_prev[i];
    return total;
}
