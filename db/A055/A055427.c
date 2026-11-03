/*
 * A055427  Number of points in Z^n of norm <= 3.
 * https://oeis.org/A055427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer vectors (x1,…,xn) ∈ ℤⁿ with * x1²+…+xn²
 *            ≤ 9. * This equals the sum of the coefficients up to degree 9 in
 *            the polynomial * (1 + 2t + 2t⁴ + 2t⁹)ⁿ. The function computes
 *            this via binary * exponentiation of the truncated polynomial
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 27, u64 n <= 290, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 896 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply two degree‑≤9 polynomials a and b, store result in res;
 * coefficients larger than degree 9 are discarded (truncated). */
static void poly_mul(const seq_t a[10], const seq_t b[10], seq_t res[10])
{
    seq_t tmp[10] = {0};
    for (int i = 0; i < 10; ++i) {
        if (a[i] == 0) continue;
        for (int j = 0; j < 10; ++j) {
            if (i + j > 9) break;
            if (b[j] == 0) continue;
            tmp[i + j] += a[i] * b[j];
        }
    }
    for (int i = 0; i < 10; ++i) res[i] = tmp[i];
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* base polynomial f(t) = 1 + 2t + 2t⁴ + 2t⁹ */
    static const seq_t base[10] = {1, 2, 0, 0, 2, 0, 0, 0, 0, 2};

    seq_t res[10]  = {0};
    seq_t power[10];   /* current power of base being squared */
    seq_t tmp[10];

    /* res = f⁰ = 1 */
    res[0] = 1;

    /* power = f */
    for (int i = 0; i < 10; ++i) power[i] = base[i];

    idx_t exp = n;
    while (exp) {
        if (exp & 1ULL) {          /* res *= power */
            poly_mul(res, power, tmp);
            for (int i = 0; i < 10; ++i) res[i] = tmp[i];
        }
        exp >>= 1ULL;               /* shift exponent */
        if (!exp) break;            /* no need to square the last time */
        /* power *= power */
        poly_mul(power, power, tmp);
        for (int i = 0; i < 10; ++i) power[i] = tmp[i];
    }

    /* sum all coefficients (degree ≤ 9) */
    seq_t total = 0;
    for (int i = 0; i < 10; ++i) total += res[i];
    return total;
}
