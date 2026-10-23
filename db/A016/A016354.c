/*
 * A016354  198th cyclotomic polynomial.
 * https://oeis.org/A016354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 198th cyclotomic polynomial
 *            Φ₁₉₈(x)
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_DEG 260  /* sufficient for intermediate polynomials */

/* Multiply polynomial by (x^k - 1): poly(new_deg) = poly(old_deg) * (x^k - 1) */
static void mult_by_binomial(seq_t *poly, size_t *deg, unsigned k)
{
    static seq_t temp[MAX_DEG];
    size_t new_deg = *deg + k;

    for (size_t i = 0; i <= new_deg; ++i) temp[i] = 0;
    for (size_t i = 0; i <= *deg; ++i) {
        seq_t val = poly[i];
        temp[i]       -= val;   /* multiply by -1 (constant term) */
        temp[i + k]   += val;   /* shift by k (x^k term) */
    }

    for (size_t i = 0; i <= new_deg; ++i) poly[i] = temp[i];
    *deg = new_deg;
}

/* Divide polynomial by (x^k - 1) exactly: poly(new_deg) = poly(old_deg) / (x^k - 1) */
static void div_by_binomial(seq_t *poly, size_t *deg, unsigned k)
{
    size_t cur_deg = *deg;
    static seq_t temp[MAX_DEG];
    static seq_t quotient[MAX_DEG];

    for (size_t i = 0; i <= cur_deg; ++i) temp[i] = poly[i];

    size_t qdeg = (cur_deg >= k) ? cur_deg - k : 0;
    for (size_t i = 0; i <= qdeg; ++i) quotient[i] = 0;

    for (int i = (int)cur_deg; i >= (int)k; --i) {
        seq_t coeff = temp[i];
        if (coeff) {
            quotient[i - k] += coeff;
            temp[i] -= coeff;          /* cancel leading term */
            temp[i - k] += coeff;      /* add the -(-coeff) part */
        }
    }

    for (size_t i = 0; i <= qdeg; ++i) poly[i] = quotient[i];

    /* trim leading zeros */
    size_t new_deg = qdeg;
    while (new_deg > 0 && poly[new_deg] == (seq_t)0) --new_deg;
    *deg = new_deg;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t coeff[61] = {0};

    if (!initialized) {
        size_t deg = 0;
        static seq_t poly[MAX_DEG];
        poly[0] = (seq_t)1;  /* start with polynomial 1 */

        /* Multiply by (x^k - 1) for exponents with µ = +1 */
        mult_by_binomial(poly, &deg, 6);
        mult_by_binomial(poly, &deg, 9);
        mult_by_binomial(poly, &deg, 33);
        mult_by_binomial(poly, &deg, 198);

        /* Divide by (x^k - 1) for exponents with µ = -1 */
        div_by_binomial(poly, &deg, 3);
        div_by_binomial(poly, &deg, 18);
        div_by_binomial(poly, &deg, 66);
        div_by_binomial(poly, &deg, 99);

        /* Store resulting coefficients up to degree 60 */
        for (size_t i = 0; i <= deg && i < 61; ++i) coeff[i] = poly[i];
        for (size_t i = deg + 1; i < 61; ++i) coeff[i] = 0;

        initialized = true;
    }

    return (n < 61) ? coeff[n] : (seq_t)0;
}
