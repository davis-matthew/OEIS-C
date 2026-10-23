/*
 * A022186  Triangle of Gaussian binomial coefficients [ n,k ] for q = 22.
 * https://oeis.org/A022186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Gaussian binomial coefficient [r,k]_{22} where r is the
 *            row satisfying r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2. The sequence is obtained by flattening the triangle
 *            of [n,k]_{22} left‑to‑right
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 38, u128 n <= 70,
 *            big any n
 * Helpers:   gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"      /* gcd(seq_t, seq_t) */
#include "../utils/seq_pow.h"   /* seq_pow(seq_t, idx_t) */

/* Compute [n,k]_{q} for q = 22. */
static seq_t gaussian_binom(idx_t n, idx_t k)
{
    if (k == 0 || k == n)
        return (seq_t)1;

    const seq_t q = (seq_t)22;
    seq_t result = (seq_t)1;

    for (idx_t i = 1; i <= k; ++i)
    {
        /* numerator = q^(n+1-i) - 1 */
        seq_t num = seq_pow(q, n + 1 - i);
        num -= (seq_t)1;

        /* denominator = q^i - 1 */
        seq_t den = seq_pow(q, i);
        den -= (seq_t)1;

        /* Cancel common factors between result and the denominator. */
        seq_t g = gcd(result, den);
        if (g != (seq_t)1) {
            result /= g;
            den /= g;
        }

        /* Cancel remaining common factors between numerator and denominator. */
        g = gcd(num, den);
        if (g != (seq_t)1) {
            num /= g;
            den /= g;
        }

        /* Multiply the reduced numerator and divide by the reduced denominator. */
        result = (result * num) / den;   /* den is 1 after cancellations. */
    }

    return result;
}

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t r = find_row(n);
    idx_t prev_terms = r * (r + 1) / 2;
    idx_t k = n - prev_terms;

    return gaussian_binom(r, k);
}
