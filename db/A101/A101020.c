/*
 * A101020  Table of numerators of coefficients of certain rational
 *          polynomials.
 * https://oeis.org/A101020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, tabl, easy
 * Formula:   a(n) = numerator of ((2^m * C(r,m)^2) / C(2m,m)), where r =
 *            ⌊(√(8n+1)−1)/2⌋, m = n – r(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   pow2_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_local.h"

/* Greatest–common divisor for the seq_t type. */
static seq_t gcd_seq_local(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Binomial coefficient C(n,k) for seq_t values. */
static seq_t binom_local(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd_seq_local(num, den);
        num /= g;
        den /= g;
        res = res * num;
        res = res / den;   /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    unsigned __int128 tri = 1;          /* (r+1)(r+2)/2 for current r */
    while (tri <= n) {
        ++r;
        tri = ((unsigned __int128)(r + 1) * (r + 2)) / 2;
    }

    /* Column index m within the row. */
    unsigned __int128 start128 = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t start = (idx_t)start128;
    idx_t m = n - start;

    if (m == (idx_t)0)
        return (seq_t)1;

    seq_t binom_rm   = binom_local(r, m);
    seq_t binom_2m_m = binom_local((idx_t)(2 * m), m);
    seq_t pow2       = pow2_local(m);

    seq_t num_raw = binom_rm * binom_rm;
    num_raw *= pow2;

    seq_t g = gcd_seq_local(num_raw, binom_2m_m);
    return num_raw / g;
}
