/*
 * A038253  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*11^j.
 * https://oeis.org/A038253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 5^(i-j) * 11^j where i = max{k | k(k+1)/2
 *            ≤ n}, j = n - i(i+1)/2. The triangle entries are enumerated
 *            row‑wise: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0),
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

/* Compute the nth term of OEIS A038253 */
seq_t a(idx_t n)
{
    /* Find the row i such that i(i+1)/2 ≤ n < (i+1)(i+2)/2 */
    idx_t lo = 0, hi = 1;
    /* Exponentially enlarge hi until the triangle index surpasses n */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;
    lo = hi >> 1;

    /* Binary search for the largest i with i(i+1)/2 ≤ n */
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t i = lo - 1;
    idx_t tri = (idx_t)((unsigned __int128)i * (i + 1) / 2);
    idx_t j = n - tri;

    /* Triangle entry T(i,j) = binomial(i,j) * 5^(i-j) * 11^j */
    seq_t val = binomial(i, j);
    val *= pow_uint(5ULL, i - j);
    val *= pow_uint(11ULL, j);
    return val;
}
