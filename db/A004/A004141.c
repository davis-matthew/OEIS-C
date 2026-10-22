/*
 * A004141  Norm of a matrix.
 * https://oeis.org/A004141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{k=0..n-1} |c(n-1,k)|, where c(m,k) =
 *            m*(m+k-1)!*4^k/((m-k)!*(2k)!) are the coefficients of the
 *            shifted Chebyshev polynomial T*_m(x) = T_m(2x-1) (row m of
 *            A127674 in absolute value); a(1) = 1. Ratio used: c(m,k+1) =
 *            c(m,k) * 2*(m+k)*(m-k) / ((2k+1)*(k+1)), c(m,0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   a004141_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004141_gcd.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n - 1;
    seq_t c = 1;
    seq_t best = 1;
    uint64_t k;
    if (m == 0)
        return best;
    for (k = 0; k < m; k++) {
        uint64_t num = 2 * (m + k) * (m - k);
        uint64_t den = (2 * k + 1) * (k + 1);
        uint64_t g = a004141_gcd(num, den);
        num /= g;
        den /= g;
        c = c / den;
        c = c * num;
        if (c > best)
            best = c;
    }
    return best;
}
