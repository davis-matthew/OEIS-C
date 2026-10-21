/*
 * A082906  Sum of terms in n-th row of modified Pascal's triangle displayed
 *          in A082905.
 * https://oeis.org/A082906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} C(n/gcd(n,j), j/gcd(n,j))
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Euclidean algorithm for idx_t; no external dependencies */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;      /* a(0) = 1 by definition */

    seq_t total = 0;
    for (idx_t j = 0; j <= n; ++j) {
        idx_t g = gcd_idx(n, j);
        idx_t m = n / g;       /* n/g */
        idx_t k = j / g;       /* j/g */
        total += binom(m, k);
    }
    return total;
}
