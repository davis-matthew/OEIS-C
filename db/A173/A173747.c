/*
 * A173747  Square array T(n, k) = v(k, n)((1)), where v(n, q) = M*v(n-1, q),
 *          M = {{0, 1, 0}, {0, 0, 1}, {8*q^3, 6*q, 0}}, with v(0, q) = {1, 1,
 *          1}, read by antidiagonals.
 * https://oeis.org/A173747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 107, u64 n <= 280,
 *            u128 n <= 786, big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A173747:
 *  a(n) is read by antidiagonals from the square array T, where
 *    T(i,j) = f(j,i+1)  (row i, column j)
 *    f(0,q) = f(1,q) = f(2,q) = 1
 *    f(n,q) = 6*q*f(n-2,q) + 8*q^3*f(n-3,q)   for n ≥ 3
 *  For a given n:
 *    let s = floor((sqrt(8*n+1)-1)/2)
 *    let r = n - s*(s+1)/2        // 0 ≤ r ≤ s
 *    row = r, col = s - r
 *    then  a(n) = f(col, row+1)
 */
#include <math.h>   /* sqrtl */
#include "../utils/seq_one.h"   /* seq_one() */

static seq_t compute_f(idx_t n_arg, seq_t q)
{
    if (n_arg <= 2) return seq_one();

    /* values for n = 0, 1, 2 */
    seq_t f_nm3 = seq_one();   /* f(n-3) */
    seq_t f_nm2 = seq_one();   /* f(n-2) */
    seq_t f_nm1 = seq_one();   /* f(n-1) -- not used directly, but kept for shift */

    seq_t f_cur, q3 = q * q * q;

    for (idx_t i = 3; i <= n_arg; ++i)
    {
        f_cur = (seq_t)6 * q * f_nm2 + (seq_t)8 * q3 * f_nm3;
        /* shift for next iteration */
        f_nm3 = f_nm2;
        f_nm2 = f_nm1;
        f_nm1 = f_cur;
    }
    return f_cur;
}

seq_t a(idx_t n)
{
    /* compute s = floor((sqrt(8*n+1)-1)/2) */
    long double d = sqrtl((long double)(8.0L * n + 1.0L));
    idx_t s = (idx_t)((d - 1.0L) / 2.0L);

    /* correct possible rounding errors */
    unsigned __int128 tri_lower = (unsigned __int128)s * (s + 1) / 2;
    if (tri_lower > n)
    {
        while (tri_lower > (unsigned __int128)n)
        {
            --s;
            tri_lower = (unsigned __int128)s * (s + 1) / 2;
        }
    }
    else
    {
        while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= (unsigned __int128)n)
        {
            ++s;
            tri_lower = (unsigned __int128)s * (s + 1) / 2;
        }
    }

    idx_t r = n - (idx_t)tri_lower;   /* offset within antidiagonal */
    idx_t row = r;
    idx_t col = s - r;
    seq_t q = (seq_t)(row + 1);        /* row + 1 for f(col, row+1) */

    return compute_f(col, q);
}
