/*
 * A000003  Number of classes of primitive positive definite binary quadratic
 *          forms of discriminant D = -4n; or equivalently the class number of
 *          the quadratic order of discriminant D = -4n.
 * https://oeis.org/A000003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = number of reduced primitive forms (A,B,C) with B^2 - 4AC
 *            = -4n, i.e. |B| <= A <= C, B >= 0 if |B| == A or A == C,
 *            gcd(A,B,C) = 1 (search over A, B)
 * Range:     u8 n <= 9381, u16 n <= 9381, u32 n <= 9381, u64 n <= 9381,
 *            u128 n <= 9381, big any n
 * Helpers:   gcd_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_ull.h"

seq_t a(idx_t n)
{
    unsigned long long N4 = 4ULL * (unsigned long long)n;
    seq_t count = 0;
    unsigned long long A;
    for (A = 1; 3ULL * A * A <= N4; A++) {
        long long B;
        for (B = -(long long)A; B <= (long long)A; B++) {
            unsigned long long absB, num, C;
            if (B & 1) continue; /* B must be even since D is even */
            absB = (unsigned long long)(B < 0 ? -B : B);
            num = absB * absB + N4;
            if (num % (4ULL * A) != 0) continue;
            C = num / (4ULL * A);
            if (C < A) continue;
            if (B < 0 && (absB == A || A == C)) continue;
            if (gcd_ull(gcd_ull(A, absB), C) != 1) continue;
            count++;
        }
    }
    return count;
}
