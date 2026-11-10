/*
 * A053629  Smallest integer where the harmonic mean of a(n) and A005279(n) is
 *          an integer.
 * https://oeis.org/A053629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s - x, where x = A005279(n) and s is the smallest divisor
 *            of 2*x^2 with s > x (HM(x,k)=2xk/(x+k) is an integer iff (x+k) |
 *            2x^2); A005279(n) found by search
 * Range:     u8 n <= 67, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool A053629_has_pair(seq_t m) {
    seq_t prev = 0;
    for (seq_t d = 1; d <= m; d++) {
        if (m % d == 0) {
            if (prev != 0 && d < 2 * prev) return true;
            prev = d;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t x = 1;
    while (count < n) {
        x++;
        if (A053629_has_pair(x)) count++;
    }
    for (seq_t s = x + 1; s < 2 * x; s++) {
        if (((2 * x % s) * (x % s)) % s == 0) return s - x;
    }
    return x;
}
