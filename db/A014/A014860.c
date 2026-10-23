/*
 * A014860  Inverse of 851st cyclotomic polynomial.
 * https://oeis.org/A014860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   let Φ_851(x) = Σ_{i=0}^{792} φ_i x^i, with φ_0 = 1. Then a(0) =
 *            1 and for n ≥ 1 a(n) = - Σ_{i=1}^{min(n,792)} φ_i · a(n-i). The
 *            sequence is periodic with period 851
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const idx_t PERIOD = 851;
static int ready = 0;

static seq_t phi_coeff[793];    /* coefficients of Φ_851(x), degree 792 */
static seq_t seq_cache[851];    /* periodic sequence values, indices 0..850 */

/* Multiply two polynomials a(0..degA) * b(0..degB) */
static void poly_mul(const seq_t *a, size_t degA, const seq_t *b,
                     size_t degB, seq_t *res, size_t resSize)
{
    for (size_t i = 0; i < resSize; ++i)
        res[i] = 0;
    for (size_t i = 0; i <= degA; ++i)
        for (size_t j = 0; j <= degB; ++j)
            res[i + j] += a[i] * b[j];
}

/* Polynomial long division: num / den, where leading coeff of den is ±1.
   Quotient array quo must be of size degNum - degDen + 1.  Remainder is discarded. */
static void poly_div(const seq_t *num, size_t degNum,
                     const seq_t *den, size_t degDen,
                     seq_t *quo, size_t quoSize)
{
    /* temp holds a mutable copy of the numerator */
    seq_t temp[853];          /* max degree 852 + 1 */
    for (size_t i = 0; i <= degNum; ++i)
        temp[i] = num[i];

    for (ptrdiff_t k = (ptrdiff_t)degNum; k >= (ptrdiff_t)degDen; --k) {
        seq_t coeff = temp[k];
        size_t qInd = (size_t)(k - (ptrdiff_t)degDen);
        quo[qInd] = coeff;
        for (size_t j = 0; j <= degDen; ++j) {
            size_t tInd = (size_t)(k - (ptrdiff_t)degDen + j);
            temp[tInd] -= coeff * den[j];
        }
    }
}

/* One‑time computation of Φ_851(x) coefficients and the sequence values. */
static void init_sequence(void)
{
    /* 1. numerator = (x-1)(x^851-1) */
    seq_t base1[2];
    seq_t base2[852];
    base1[0] = -1;   base1[1] = 1;          /* x-1       */
    for (idx_t i = 0; i < 852; ++i) base2[i] = 0;   /* initialize */
    base2[0]   = -1;
    base2[851] = 1;                         /* x^851-1   */
    seq_t num[853];
    poly_mul(base1, 1, base2, 851, num, 853);

    /* 2. denominator = (x^23-1)(x^37-1) */
    seq_t part1[24], part2[38];
    for (idx_t i = 0; i < 24; ++i) part1[i] = 0;   /* x^23-1   */
    part1[0] = -1;  part1[23] = 1;
    for (idx_t i = 0; i < 38; ++i) part2[i] = 0;   /* x^37-1   */
    part2[0] = -1;  part2[37] = 1;
    seq_t denom[61];
    poly_mul(part1, 23, part2, 37, denom, 61);

    /* 3. divide to obtain Φ_851(x) */
    poly_div(num, 852, denom, 60, phi_coeff, 793);

    /* 4. compute sequence values a(n) for n = 0 .. 850 */
    seq_cache[0] = 1;  /* a(0) = 1 */
    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t limit = (n < 792) ? (size_t)n : 792;
        for (size_t i = 1; i <= limit; ++i)
            sum += phi_coeff[i] * seq_cache[(size_t)(n - i)];
        seq_cache[(size_t)n] = -sum;
    }

    ready = 1;
}

seq_t a(idx_t n)
{
    if (!ready)
        init_sequence();
    return seq_cache[(size_t)(n % PERIOD)];
}
