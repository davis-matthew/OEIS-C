/*
 * A082292  a(n) = (2*n+1) * (2*n)! / (sqrt(4*(n+1)*Product_{k=1..2*n+1}
 *          lcm(k, 2*n+2-k))).
 * https://oeis.org/A082292
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=1..n} gcd(k, 2n+2-k) / 2
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    // For n < 2 the behaviour is irrelevant, but a small guard is harmless.
    if (n < 1) return (seq_t)0;

    idx_t M = 2 * n + 2;          // M = 2n + 2
    seq_t prod = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = gcd((seq_t)k, (seq_t)(M - k));
        prod *= g;
    }

    prod /= (seq_t)2;             // division is exact for n >= 2
    return prod;
}
