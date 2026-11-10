/*
 * A093920  Row sums of A093919.
 * https://oeis.org/A093920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} lcm(n, n-1, ..., n-k+1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t l = 1;
    seq_t sum = 0;
    for (idx_t i = 0; i < n; i++) {
        seq_t term = (seq_t)(n - i);
        seq_t g = gcd(l, term);
        l = l / g * term;
        sum += l;
    }
    return sum;
}
