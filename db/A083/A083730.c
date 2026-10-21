/*
 * A083730  Greatest prime^2 factor of n, or a(n)=1 for squarefree n.
 * https://oeis.org/A083730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest prime squared dividing n, or 1 if n is
 *            squarefree
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    uint64_t m = (uint64_t)n;
    uint64_t remaining = m;
    uint64_t best = 1;

    /* trial division to find prime factors of n */
    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            unsigned exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            if (exp >= 2) {
                uint64_t p2 = p * p;
                if (p2 > best) best = p2;
            }
        }
    }
    /* remaining > 1 is a prime factor with exponent 1, not a square */

    return (seq_t)best;
}
