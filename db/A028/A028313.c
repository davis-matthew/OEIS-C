/*
 * A028313  Elements in the 5-Pascal triangle (by row).
 * https://oeis.org/A028313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) + 3*binomial(r-2,k-1) where r and k are the
 *            row and column indices of the n‑th element of the 5‑Pascal
 *            triangle. Base values: a(0)=a(1)=1
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 644, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute C(n,k) for nonnegative n,k using only seq_t arithmetic.*/
static seq_t binom_idx (idx_t n, idx_t k)
{
    if (k > n)          return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k)      k = n - k;          /* symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd(num, den);   /* cancel num / den */
        num /= g;
        den /= g;

        g = gcd(res, den);          /* cancel res / den */
        res /= g;
        den /= g;

        res *= num;                 /* res = res*num/den (den divides res*num) */
        res /= den;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Determine the row r and column k of the n‑th element.
       The number of terms before row r is r*(r+1)/2. */
    idx_t r = 0;
    while ( ( (r + 1) * (r + 2) ) / 2 <= n )  ++r;

    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;          /* 0‑based column in row r */

    if (r <= 1)          return 1;   /* base rows */

    seq_t val = binom_idx(r, k);
    if (k > 0) {
        seq_t add = binom_idx(r - 2, k - 1);
        val += 3 * add;
    }
    return val;
}
