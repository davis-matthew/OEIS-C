/*
 * A003418  Least common multiple (or LCM) of {1, 2, ..., n} for n >= 1, a(0)
 *          = 1.
 * https://oeis.org/A003418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = lcm(1,2,3,...,n), with a(0) = 1, computed as a(n) =
 *            lcm(n, a(n-1))
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static seq_t lcm_seq(seq_t a, seq_t b) {
    if (a == 0 || b == 0) return 0;
    seq_t g = gcd_seq(a, b);
    return a / g * b;
}

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    seq_t result = 1;
    for (idx_t i = 2; i <= n; i++) {
        result = lcm_seq(result, (seq_t)i);
    }
    return result;
}
