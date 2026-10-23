/*
 * A023770  Nialpdromes: digits in base 15 are in nonincreasing order.
 * https://oeis.org/A023770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th non‑negative integer whose base‑15 digits are
 *            weakly non‑increasing (each digit ≤ the preceding one). The
 *            function enumerates the sequence combinatorially
 * Range:     u8 n <= 137, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Zero‑based index among the positive terms. */
    seq_t target = (seq_t)n - (seq_t)2;

    /* Determine the number of base‑15 digits (length m). */
    unsigned int m = 1;
    seq_t cum = (seq_t)0;      /* cumulative count of lengths < m */
    for (;;) {
        seq_t count_m = binom((idx_t)(m + 14), (idx_t)14) - (seq_t)1;
        if (cum + count_m > target) {
            break;
        }
        cum += count_m;
        ++m;
    }

    /* Index within the block of numbers having length m. */
    seq_t k = target - cum;

    /* Build the number digit by digit. */
    unsigned int remaining = m;
    unsigned int prev_digit = 14;   /* maximum allowed digit for the next position */
    seq_t value = (seq_t)0;

    for (unsigned int pos = 0; pos < m; ++pos) {
        unsigned int min_digit = (pos == 0) ? 1 : 0;
        unsigned int max_digit = prev_digit;
        unsigned int digit;

        for (digit = min_digit; digit <= max_digit; ++digit) {
            seq_t cnt = binom((idx_t)(remaining - 1 + digit), (idx_t)digit);
            if (k < cnt) {
                /* Select this digit. */
                value = value * (seq_t)15 + (seq_t)digit;
                prev_digit = digit;
                break;
            }
            k -= cnt;
        }
        --remaining;
    }

    return value;
}
