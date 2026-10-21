/*
 * A078471  Sum of all odd divisors of all positive integers <= n.
 * https://oeis.org/A078471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d odd ≤ n} d * floor(n/d)
 * Range:     u8 n <= 24, u16 n <= 398, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalent to a(n) = Sum_{k=0..n} (2k+1)*floor(n/(2k+1)) ) */
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t d = 1; d <= n; d += 2u) {
        seq_t dd   = (seq_t)d;
        seq_t times = (seq_t)(n / d);
        result += dd * times;
    }
    return result;
}
