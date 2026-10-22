/*
 * A003838  Order of (usually) simple Chevalley group D_n (5).
 * https://oeis.org/A003838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(5,n) = 5^(n*(n-1)) * (5^n - 1) * Product_{i=1..n-1}
 *            (5^(2*i) - 1) / gcd(4, 5^n - 1), where gcd(4, 5^n - 1) = 4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a003838_pow5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003838_pow5.h"

seq_t a(idx_t n) {
    /* 5^n - 1 is always divisible by 4, so divide first to keep intermediates small */
    seq_t result = (a003838_pow5(n) - 1) / 4;
    seq_t p2 = 1;
    for (idx_t i = 1; i < n; i++) {
        p2 *= 25;
        result *= (p2 - 1);
    }
    result *= a003838_pow5(n * (n - 1));
    return result;
}
