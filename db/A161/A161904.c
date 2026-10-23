/*
 * A161904  Array read by rows in which row n list the divisors of n, but if n
 *          is a square then the square root of n appears repeated. Also, the
 *          divisors appear as pairs (a,b), where a <= b, such that a*b = n.
 * https://oeis.org/A161904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A161904 – read the divisors of n in pairs (a,b),
 * where a ≤ b and a*b = n.  For square n, the pair (√n,√n) appears twice.
 * The sequence is obtained by concatenating, for n = 1,2,3,…,
 * the list a, b, a, b, … for each divisor pair in ascending order of a.
 * Example: n = 4 → pairs (1,4), (2,2) → terms 1,4,2,2.
 */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Ensure n is at least 1 (out of range values are undefined). */
    if (n < 1) return 0;

    idx_t row = 1;
    idx_t cumulative = 0;

    while (1) {
        /* Count divisors and determine if row is a perfect square. */
        idx_t dcnt = 0;
        bool is_square = false;

        for (idx_t d = 1; d <= row / d; ++d) {
            if (row % d == 0) {
                if (d == row / d) {
                    dcnt += 1;
                    is_square = true;
                } else {
                    dcnt += 2;
                }
            }
        }

        /* Row length = dcnt + 1 if square else dcnt. */
        idx_t row_len = dcnt + (is_square ? 1 : 0);

        if (cumulative + row_len >= n) {
            idx_t pos = n - cumulative - 1; /* 0‑based position within the row */
            idx_t pair_idx = pos / 2;
            idx_t sub_pos = pos % 2;      /* 0 → a, 1 → b */

            /* Locate the divisor pair at pair_idx. */
            idx_t found = 0;
            for (idx_t d = 1; d <= row / d; ++d) {
                if (row % d == 0) {
                    if (found == pair_idx) {
                        seq_t a_val = (seq_t)d;
                        seq_t b_val = (seq_t)(row / d);
                        return (sub_pos == 0) ? a_val : b_val;
                    }
                    found++;
                }
            }
            /* Should never reach here */
            return 0;
        }

        cumulative += row_len;
        row++;
    }
}
