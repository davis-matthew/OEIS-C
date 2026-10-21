/*
 * A072300  Number of positive integers not exceeding n that are relatively
 *          prime to reverse(n).
 * https://oeis.org/A072300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = #{1 ≤ m ≤ n | gcd(m , reverse(n)) = 1}
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Direct evaluation: reverse(n) once, then count m up to n whose gcd with it is 1. */
#include "../utils/gcd.h"

static seq_t reverse_decimal(seq_t x) {
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x = x / 10;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t rev_n = reverse_decimal((seq_t)n);
    seq_t count = 0;
    for (idx_t m = 1; m <= n; ++m) {
        if (gcd((seq_t)m, rev_n) == (seq_t)1)
            ++count;
    }
    return count;
}
