/*
 * A069542  Duplicate of A063416.
 * https://oeis.org/A069542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 15, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers divisible by 7 and having decimal digit sum 7 */
/* a(n) = the nth positive integer m such that m ≡ 0 (mod 7) and sum_of_digits(m) = 7 */

#include <stdint.h>

/* Compute the decimal digit sum of an unsigned integer.  Result fits in seq_t. */
static seq_t digit_sum_idx(idx_t x) {
    seq_t sum = 0;
    while (x > 0) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* n is 1‑based; the caller guarantees n ≥ 1. */
    idx_t cand = 7;              /* first multiple of 7 */
    idx_t count = 0;

    while (count < n) {
        if (digit_sum_idx(cand) == 7) {
            count++;
            if (count == n) {
                return (seq_t)cand;   /* cast to seq_t only when needed */
            }
        }
        cand += 7;
    }
    /* unreachable, but required to avoid a warning about missing return */
    return 0;
}
