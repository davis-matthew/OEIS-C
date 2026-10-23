/*
 * A194120  Triangular array: T(n,k)=C(n+4,k)+C(n+4,k+4), 0<=k<=n.
 * https://oeis.org/A194120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangular array * T(r,k) =
 *            C(r+4,k) + C(r+4,k+4) (0 ≤ k ≤ r), * arranged by concatenating
 *            the rows r = 0,1,2,... * For a given linear index n we first
 *            determine the row * r by solving r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
 *            and then * k = n – r*(r+1)/2. Finally a(n) = C(r+4,...
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(res, den);
        res /= g;
        den /= g;

        /* After the reductions den should be 1. */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = 0, S = 0;
    /* Find the smallest r with S + (r+1) > n   (i.e., S = r*(r+1)/2) */
    while (n >= S + (r + 1))
    {
        S += r + 1;
        ++r;
    }
    idx_t k = n - S;

    seq_t val1 = binom_multiplicative(r + 4, k);
    seq_t val2 = binom_multiplicative(r + 4, k + 4);
    return val1 + val2;
}
