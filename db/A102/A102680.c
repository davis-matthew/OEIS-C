/*
 * A102680  Number of digits >= 7 in the decimal representations of all
 *          integers from 0 to n.
 * https://oeis.org/A102680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = total number of decimal digits ≥ 7 in all integers from 0
 *            to n
 * Range:     u8 n <= 456, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements digit‑DP counting for each digit position. */
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t factor = 1; factor <= n; ) {
        /* high part : digits left of current position */
        idx_t high;
        if (factor > n / 10) {
            high = 0;
        } else {
            high = n / (factor * 10);
        }
        /* current digit at this position */
        idx_t cur_digit = (n / factor) % 10;
        /* low part : digits right of current position */
        idx_t low = n % factor;

        /* count occurrences of digits 7, 8, 9 at this position */
        for (int d = 7; d <= 9; ++d) {
            seq_t add;
            if (cur_digit > (idx_t)d) {
                add = ((seq_t)(high + 1)) * (seq_t)factor;
            } else if (cur_digit == (idx_t)d) {
                add = ((seq_t)high) * (seq_t)factor + (seq_t)low + 1;
            } else {
                add = ((seq_t)high) * (seq_t)factor;
            }
            total += add;
        }

        if (factor > n / 10) break;   /* avoid overflow when multiplying by 10 */
        factor *= 10;
    }
    return total;
}
