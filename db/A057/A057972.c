/*
 * A057972  Number of 5 X n binary matrices with exactly 3 singleton columns
 *          up to row and column permutations.
 * https://oeis.org/A057972
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/120 * ( 35*C(n+23,26) + 130*∑_{j=0}^{⌊(n-3)/2⌋}
 *            C(n-3-2j+12,12)*C(j+6,6) + 45*∑_{j=0}^{⌊(n-3)/2⌋}
 *            C(n-3-2j+6,6)*C(j+9,9) + 100*∑_{k=0}^{⌊(n-3)/3⌋}
 *            C(n-3-3k+5,5)*C(k+6,6) + 20*coeff of x^{n-3} in
 *            (1‑x)⁻⁴(1‑x²)⁻¹(1‑x³)⁻³(1‑x⁶)⁻² +
 *            30*∑_{k=0}^{⌊(n-3)/4⌋}∑_{j=0}^{⌊(n-3-4k)/2⌋} C(n-3-4k-2j+2,...
 * Range:     u8 none, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Helper: binomial (k+r choose r) for small r (<=26). */
static seq_t binom_small(idx_t k, int r)
{
    seq_t res = 1;
    for (int i = 1; i <= r; ++i) {
        res = res * (seq_t)(k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Cached temporary arrays (max size for n up to ≈10000). */
#define MAX_N 10000
static seq_t F0[MAX_N + 1];
static seq_t F1[MAX_N + 1];
static seq_t F2[MAX_N + 1];
static seq_t F3[MAX_N + 1];

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* behaviour below offset irrelevant */
    idx_t m = n - 3;              /* exponent of x in the series expansion */

    /* ---------- term 1 ---------- */
    seq_t term1 = 35 * binom_small(m, 26);   /* C(m+26,26) */

    /* ---------- term 2 ----------
       ∑ C(m-2j+12,12) * C(j+6,6)   j ≤ m/2  */
    seq_t term2 = 0;
    for (idx_t j = 0; j <= m / 2; ++j) {
        idx_t i = m - 2 * j;
        term2 += binom_small(i, 12) * binom_small(j, 6);
    }
    term2 *= 130;

    /* ---------- term 3 ----------
       ∑ C(m-2j+6,6) * C(j+9,9)    j ≤ m/2  */
    seq_t term3 = 0;
    for (idx_t j = 0; j <= m / 2; ++j) {
        idx_t i = m - 2 * j;
        term3 += binom_small(i, 6) * binom_small(j, 9);
    }
    term3 *= 45;

    /* ---------- term 4 ----------
       ∑ C(m-3k+5,5) * C(k+6,6)    k ≤ m/3  */
    seq_t term4 = 0;
    for (idx_t k = 0; k <= m / 3; ++k) {
        idx_t i = m - 3 * k;
        term4 += binom_small(i, 5) * binom_small(k, 6);
    }
    term4 *= 100;

    /* ---------- term 5 ----------
       convolution of (1-x)^-4, (1-x^2)^-1, (1-x^3)^-3, (1-x^6)^-2 */
    /* allocate only up to needed size (m+1) */
    size_t sz = (size_t)m + 1;
    if (sz > (size_t)MAX_N + 1) sz = (size_t)MAX_N + 1;  /* safety */

    /* F0: (1-x)^-4  -> C(i+3,3)  */
    for (size_t i = 0; i < sz; ++i) F0[i] = binom_small((idx_t)i, 3);

    /* F1: convolve with (1-x^2)^-1, coefficient 1 at even positions */
    for (size_t i = 0; i < sz; ++i) {
        F1[i] = F0[i];
        if (i >= 2) F1[i] += F1[i-2];
    }

    /* F2: convolve with (1-x^3)^-3, coeff C(t+2,2) at positions 3t */
    for (size_t i = 0; i < sz; ++i) F2[i] = 0;
    for (size_t t = 0; t <= m / 3; ++t) {
        seq_t coeff = binom_small((idx_t)t, 2);  /* C(t+2,2) */
        size_t shift = 3 * t;
        for (size_t i = 0; i + shift < sz; ++i) {
            F2[i + shift] += F1[i] * coeff;
        }
    }

    /* F3: convolve with (1-x^6)^-2, coeff (s+1) at positions 6s */
    for (size_t i = 0; i < sz; ++i) F3[i] = 0;
    for (size_t s = 0; s <= m / 6; ++s) {
        seq_t coeff = (seq_t)(s + 1);           /* C(s+1,1) = s+1 */
        size_t shift = 6 * s;
        for (size_t i = 0; i + shift < sz; ++i) {
            F3[i + shift] += F2[i] * coeff;
        }
    }

    seq_t term5 = 20 * F3[(size_t)m];
    /* ---------- term 6 ----------
       ∑_{k=0}^{⌊m/4⌋} ∑_{j=0}^{⌊(m-4k)/2⌋}
         C(m-4k-2j+2,2) * C(j+1,1) * C(k+4,4) */
    seq_t term6 = 0;
    for (idx_t k = 0; k <= m / 4; ++k) {
        idx_t km = m - 4 * k;
        seq_t coeff_k = binom_small(k, 4);     /* C(k+4,4) */
        for (idx_t j = 0; j <= km / 2; ++j) {
            idx_t ij = km - 2 * j;
            seq_t coeff_i = binom_small(ij, 2); /* C(ij+2,2) */
            seq_t coeff_j = (seq_t)(j + 1);     /* C(j+1,1) */
            term6 += coeff_i * coeff_j * coeff_k;
        }
    }
    term6 *= 30;

    /* Sum all terms and divide by 120 */
    seq_t total = term1 + term2 + term3 + term4 + term5 + term6;
    return total / (seq_t)120;
}
