/*
 * A006481  Euler characteristics of polytopes.
 * https://oeis.org/A006481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = C(m, floor(m/2)) + 1 if m is not divisible by 4, and C(m,
 *            floor((m-1)/2)) + 1 otherwise, where m = n-1; a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 978 (2026-11-01)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    idx_t m, k;
    if (n <= 1) return (seq_t)1;
    m = n - 1;
    if (m % 4 == 0) k = (m - 1) / 2;
    else k = m / 2;
    return binomial(m, k) + 1;
}
