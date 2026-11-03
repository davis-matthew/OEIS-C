/*
 * A084887  Triangular array, read by rows: T(n,k) = denominator of arithmetic
 *          derivative of k/n, 1<=k<=n.
 * https://oeis.org/A084887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A084887: a(n) = denominator of the arithmetic derivative of k/n,
   where the index n is mapped to the pair (m,k) in the triangular array
   (m = row, k = column, 1‑based). This value is computed from
   D(k/n) = (m*A003415(k) – k*A003415(m)) / m², taking the denominator
   in lowest terms. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"

/* Arithmetic derivative of an integer n (n≥0).  The result fits in 64 bits
   for all n used in this problem (n≤10⁴ in typical tests). */
static inline unsigned long long arithmetic_derivative_uint64(idx_t n)
{
    if (n <= 1)
        return 0;

    unsigned long long sum = 0;
    unsigned long long m   = n;
    unsigned long long p   = 2;

    while (p * p <= m) {
        if (m % p == 0) {
            unsigned long long exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* contribution: n * exp / p   (division exact) */
            sum += ((unsigned long long)n) * exp / p;
        }
        ++p;
    }
    if (m > 1)          /* remaining prime factor */
        sum += ((unsigned long long)n) / m;

    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 1. Find row m and column k such that
       n = 1 + 2 + … + m – (m-1) + k = (m*(m-1))/2 + k  with 1≤k≤m */
    idx_t m = 1;
    while ((m * (m + 1)) / 2 < n)
        ++m;
    idx_t prev = (m - 1) * m / 2;
    idx_t k    = n - prev;

    /* 2. Compute arithmetic derivatives of m and k. */
    unsigned long long d_m = arithmetic_derivative_uint64(m);
    unsigned long long d_k = arithmetic_derivative_uint64(k);

    /* 3. Compute numerator of D(k/m). */
    __int128 num = (__int128)m * (__int128)d_k - (__int128)k * (__int128)d_m;
    if (num == 0)
        return (seq_t)1;                /* derivative is zero → denom 1 */

    unsigned long long abs_num = (num >= 0) ? (unsigned long long)num
                                            : (unsigned long long)(-num);

    /* 4. Denominator before reduction: m². */
    unsigned long long m_sq = (unsigned long long)m * (unsigned long long)m;

    /* 5. Reduce fraction: denom = m² / gcd(abs_num, m²). */
    unsigned long long g = gcd_u64(abs_num, m_sq);
    unsigned long long denom = m_sq / g;

    return (seq_t)denom;
}
