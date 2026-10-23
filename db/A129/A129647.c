/*
 * A129647  Largest order of a permutation of n elements with exactly 2
 *          cycles. Also the largest LCM of a 2-partition of n.
 * https://oeis.org/A129647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{i=1..n-1} lcm(i, n-i) = max_{i} i*(n-i)/gcd(i, n-i)
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return 0;
    seq_t best = 0;
    for (idx_t i = 1; i < n; ++i) {
        seq_t a_i = (seq_t)i;
        seq_t b_i = (seq_t)(n - i);
        seq_t g = gcd(a_i, b_i);
        seq_t val = (a_i * b_i) / g;
        if (val > best)
            best = val;
    }
    return best;
}
