/*
 * A022997  Numerator of n*(n-2)*(2*n-1)/(2*(n-1)).
 * https://oeis.org/A022997
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of n*(n-2)*(2*n-1) / (2*(n-1))
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 1290, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 2)
        return 0;                // undefined for n < 2, just return 0
    seq_t N = (seq_t)n;
    seq_t B = (seq_t)n - 2;
    seq_t C = 2 * N - 1;
    seq_t D = 2 * ((seq_t)n - 1);

    seq_t g;
    g = gcd(N, D); N /= g; D /= g;
    g = gcd(B, D); B /= g; D /= g;
    g = gcd(C, D); C /= g; D /= g;  // D should now be 1

    return N * B * C;
}
