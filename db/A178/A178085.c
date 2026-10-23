/*
 * A178085  Triangle t(n,m) = 1 - phi(n) + phi(m) + phi(n - m) read by rows,
 *          0<=m<=n.
 * https://oeis.org/A178085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = 1 - phi(r) + phi(m) + phi(r-m) where r is the row such
 *            that r(r+1)/2 <= n < (r+1)(r+2)/2 and m = n - r(r+1)/2. Here phi
 *            is Euler's totient function
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the integer square root floor */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = n;
    idx_t y = (x + (n / x)) >> 1;
    while (y < x) {
        x = y;
        y = (x + (n / x)) >> 1;
    }
    return x;
}

/* Euler's totient function for the range we need (0 <= m <= ~10000) */
static inline seq_t phi_u64(idx_t m)
{
    if (m == 0) return (seq_t)0;
    if (m == 1) return (seq_t)1;
    seq_t result = (seq_t)m;
    idx_t temp = m;
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= (seq_t)temp; ++p) {
        if (temp % p == 0) {
            result = result / p * (p - 1);
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) result = result / temp * (temp - 1);
    return result;
}

/* Find largest r such that r*(r+1)/2 <= n.  n is zero‑based index. */
static inline idx_t find_row(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = isqrt_u64(n * 2) + 2; /* rough upper bound */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n) lo = mid + 1;
        else hi = mid;
    }
    return lo - 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t start = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t m = n - start;

    seq_t phi_r  = phi_u64(r);
    seq_t phi_m  = phi_u64(m);
    seq_t phi_rm = phi_u64(r - m);

    return (seq_t)1 - phi_r + phi_m + phi_rm;
}
