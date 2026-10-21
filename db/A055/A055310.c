/*
 * A055310  Number of labeled rooted trees with n nodes and 9 leaves.
 * https://oeis.org/A055310
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! *
 *            (n-9)*(n-8)*(n-7)*(n-6)*(n-5)*(n-4)*(n-3)*(n-2)*(n-1) * (135*n^7
 *            - 7245*n^6 + 164745*n^5 - 2054535*n^4 + 15148840*n^3 -
 *            65892420*n^2 + 156108464*n - 154828800) / 505658474496000 where
 *            a(10)=10,
 * Range:     u8 none, u16 none, u32 none, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Off‑by‑ten index: a(10) is first defined value. Return 0 for n<10
       (the behavior for n<10 does not matter). */
    if (n < 10u) return (seq_t)0;

    const seq_t den = 505658474496000; /* denominator */
    seq_t result   = (seq_t)1;         /* running numerator */
    seq_t rem_den  = den;              /* remaining denominator to cancel */

    /* multiply n! part: product of 2..n */
    for (idx_t i = 2; i <= n; ++i) {
        result *= (seq_t)i;
        seq_t g = gcd(result, rem_den);
        if (g > (seq_t)1) {
            result /= g;
            rem_den /= g;
        }
    }

    /* multiply product of (n-9)..(n-1) */
    for (idx_t k = 1; k <= 9; ++k) {
        seq_t term = (seq_t)(n - k);
        result *= term;
        seq_t g = gcd(result, rem_den);
        if (g > (seq_t)1) {
            result /= g;
            rem_den /= g;
        }
    }

    /* compute the degree-7 polynomial using Horner’s method */
    seq_t nn   = (seq_t)n;
    seq_t poly = (seq_t)135 * nn * nn * nn * nn * nn * nn * nn; /* 135*n^7 */
    poly = poly
          - (seq_t)7245 * nn * nn * nn * nn * nn * nn
          + (seq_t)164745 * nn * nn * nn * nn * nn
          - (seq_t)2054535 * nn * nn * nn * nn
          + (seq_t)15148840 * nn * nn * nn
          - (seq_t)65892420 * nn * nn
          + (seq_t)156108464 * nn
          - (seq_t)154828800;

    result *= poly;
    seq_t g = gcd(result, rem_den);
    if (g > (seq_t)1) {
        result /= g;
        rem_den /= g;
    }

    /* final division by any remaining part of the denominator */
    return result / rem_den;
}
