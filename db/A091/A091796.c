/*
 * A091796  Smallest k such that for 0 <= i < n, n*(n+1)/2-i divides k+i.
 * https://oeis.org/A091796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal k such that for all 0 <= i < n, n*(n+1)/2-i
 *            divides k+i
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t L = 1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t d = T - (seq_t)i;
        seq_t g = gcd(L, d);
        seq_t tmp = L / g;
        L = tmp * d;
    }
    return L - T;
}
