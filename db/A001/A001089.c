/*
 * A001089  Number of permutations of [n] containing exactly 2 increasing
 *          subsequences of length 3.
 * https://oeis.org/A001089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   D-finite recurrence (n+5)*(n-4)*(59*n^2-n+42)*a(n) =
 *            2*(n-1)*(2*n-3)*(59*n^2+117*n+100)*a(n-1), a(4) = 3, a(n) = 0
 *            for n < 4
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   a001089_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001089_gcd.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;
    seq_t r = 3;
    idx_t k;
    for (k = 5; k <= n; k++) {
        uint64_t P = 2 * (uint64_t)(k - 1) * (uint64_t)(2 * k - 3);
        uint64_t P2 = 59 * (uint64_t)k * (uint64_t)k + 117 * (uint64_t)k + 100;
        uint64_t Q = (uint64_t)(k + 5) * (uint64_t)(k - 4);
        uint64_t Q2 = 59 * (uint64_t)k * (uint64_t)k - (uint64_t)k + 42;
        uint64_t g;
        g = a001089_gcd(P, Q);  P /= g;  Q /= g;
        g = a001089_gcd(P, Q2); P /= g;  Q2 /= g;
        g = a001089_gcd(P2, Q); P2 /= g; Q /= g;
        g = a001089_gcd(P2, Q2); P2 /= g; Q2 /= g;
        /* now Q*Q2 divides r exactly */
        {
            seq_t d1 = (seq_t)Q;
            seq_t d2 = (seq_t)Q2;
            if (d1 == 0 || d2 == 0) return 0; /* value cannot fit anyway */
            r = r / d1;
            r = r / d2;
            r = r * (seq_t)P;
            r = r * (seq_t)P2;
        }
    }
    return r;
}
