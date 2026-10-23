/*
 * A037327  Numbers whose base-6 and base-7 expansions have the same digit
 *          sum.
 * https://oeis.org/A037327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer whose base‑6 and base‑7 digit sums are equal. */
#include <stdint.h>

static seq_t digit_sum_base(seq_t x, unsigned base) {
    seq_t sum = 0;
    seq_t b = (seq_t)base;
    while (x) {
        sum += x % b;
        x /= b;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    seq_t cand = 1;
    idx_t cnt = 0;
    while (1) {
        if (digit_sum_base(cand, 6) == digit_sum_base(cand, 7)) {
            cnt++;
            if (cnt == n) return cand;
        }
        cand++;
    }
}
