/*
 * A024379  a(n) = 3rd elementary symmetric function of the first n+2 positive
 *          integers congruent to 1 mod 4.
 * https://oeis.org/A024379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2*n+3)*(n+2)*(n+1)*(4*n^2+8*n-3)/6
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t num[5];

    num[0] = sn;                      // n
    num[1] = (seq_t)(2 * sn + 3);     // 2n + 3
    num[2] = sn + 2;                  // n + 2
    num[3] = sn + 1;                  // n + 1
    num[4] = sn * sn * 4 + sn * 8 - 3; // 4n^2 + 8n - 3

    seq_t denom = 6;
    for (size_t i = 0; i < 5 && denom > 1; ++i) {
        seq_t g = gcd(num[i], denom);
        if (g > 1) {
            num[i] /= g;
            denom /= g;
        }
    }

    seq_t result = 1;
    for (size_t i = 0; i < 5; ++i)
        result *= num[i];

    /* denom should be 1 after the reductions; keep a safeguard */
    if (denom > 1)
        result /= denom;

    return result;
}
