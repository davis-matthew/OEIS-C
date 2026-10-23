/*
 * A023771  Nialpdromes: digits in base 16 are in nonincreasing order.
 * https://oeis.org/A023771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose hexadecimal digits are in nonincreasing
 *            order
 * Range:     u8 n <= 151, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline int is_nialpdrome(seq_t x) {
    if (x == 0) return 1;
    seq_t prev = x % ((seq_t)16);
    x /= ((seq_t)16);
    while (x != 0) {
        seq_t curr = x % ((seq_t)16);
        if (curr < prev) return 0;
        prev = curr;
        x /= ((seq_t)16);
    }
    return 1;
}

seq_t a(idx_t n) {
    seq_t val = 0;
    idx_t count = 0;
    while (count < n) {
        if (is_nialpdrome(val)) count++;
        if (count == n) return val;
        val++;
    }
    return val; /* should never be reached */
}
