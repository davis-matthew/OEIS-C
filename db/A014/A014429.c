/*
 * A014429  Inverse of 420th cyclotomic polynomial.
 * https://oeis.org/A014429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = -sum_{k=1}^{min(n,96)} c_k *
 *            a(n-k), where c_k are the coefficients of Φ_420(x)=∑_{k=0}^{96}
 *            c_k x^k. The sequence is periodic with period 420
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Polynomial structure.  The largest possible degree during the
   computation is 420 (for the product of all previous Φ_f's). */
typedef struct {
    size_t deg;
    seq_t coeff[421];
} Poly;

/* Divisors of 420 in increasing order. */
static const idx_t divisors[24] = {
    1, 2, 3, 4, 5, 6, 7,
    10, 12, 14, 15, 20, 21, 28,
    30, 35, 42, 60, 70, 84,
    105, 140, 210, 420
};

/* True if a divides b. */
static int divides(idx_t a, idx_t b)
{
    return (b % a) == 0;
}

/* Multiply polynomials a and b; store in res.  The arrays are large
   enough to hold all intermediate degrees (up to 420). */
static void poly_mul(Poly *res, const Poly *a, const Poly *b)
{
    size_t i, j;
    for (i = 0; i <= a->deg + b->deg; ++i) res->coeff[i] = 0;
    res->deg = a->deg + b->deg;
    for (i = 0; i <= a->deg; ++i) {
        seq_t ai = a->coeff[i];
        for (j = 0; j <= b->deg; ++j) {
            res->coeff[i + j] += ai * b->coeff[j];
        }
    }
}

/* Long division: res = num / den.  It is assumed den divides num
   exactly, and den is monic. */
static void poly_divide(Poly *res, const Poly *num, const Poly *den)
{
    int i, j;
    Poly tmp;

    /* copy numerator into tmp */
    for (i = 0; i <= (int)num->deg; ++i) tmp.coeff[i] = num->coeff[i];
    tmp.deg = num->deg;

    /* initialise result coefficients to zero */
    for (i = 0; i <= (int)num->deg; ++i) res->coeff[i] = 0;

    for (i = (int)num->deg; i >= (int)den->deg; --i) {
        seq_t coeff = tmp.coeff[i];
        res->coeff[i - (int)den->deg] = coeff;
        for (j = 0; j <= (int)den->deg; ++j) {
            tmp.coeff[i - (int)den->deg + j] -= coeff * den->coeff[j];
        }
    }
    res->deg = num->deg - den->deg;
}

/* Compute Φ_420(x) and store its coefficients in c[0..96] */
static void compute_phi420_coeffs(seq_t *c)
{
    Poly polys[24];
    size_t i, j, k;

    /* Build each Φ_d in ascending order of d. */
    for (i = 0; i < 24; ++i) {
        idx_t d = divisors[i];

        /* P(x) = x^d - 1. */
        Poly P;
        for (k = 0; k <= d; ++k) P.coeff[k] = 0;
        P.deg = d;
        P.coeff[0] = (seq_t)-1;
        P.coeff[d] = (seq_t)1;

        /* Q(x) = product of all Φ_f for f|d, f<d. */
        Poly Q;
        for (k = 0; k <= d; ++k) Q.coeff[k] = 0;
        Q.deg = 0;
        Q.coeff[0] = (seq_t)1;  /* start with 1 */

        for (j = 0; j < i; ++j) {
            if (divides(divisors[j], d)) {
                Poly prod;
                poly_mul(&prod, &Q, &polys[j]);
                Q = prod;    /* copy product into Q */
            }
        }

        /* Φ_d(x) = P(x) / Q(x). */
        Poly R;
        poly_divide(&R, &P, &Q);
        polys[i] = R;
    }

    /* Extract coefficients of Φ_420 (divisor index 23). */
    const Poly *phi420 = &polys[23];
    for (k = 0; k <= phi420->deg; ++k) c[k] = phi420->coeff[k];
    /* The degree should be 96. In case it is less, pad with zeros. */
    for (k = phi420->deg + 1; k <= 96; ++k) c[k] = 0;
}

/* Public function requested in the assignment. */
seq_t a(idx_t n)
{
    static int init = 0;
    static seq_t coeffs[420];
    static seq_t phi[97];

    if (!init) {
        /* Compute Φ_420 coefficients once. */
        compute_phi420_coeffs(phi);

        /* Generate the first 420 terms using the recurrence. */
        coeffs[0] = (seq_t)1;
        size_t i, k;
        for (i = 1; i < 420; ++i) {
            seq_t sum = 0;
            size_t maxk = (i < 97 ? i : 96);
            for (k = 1; k <= maxk; ++k) {
                sum += phi[k] * coeffs[i - k];
            }
            coeffs[i] = -sum;
        }
        init = 1;
    }
    return coeffs[(size_t)(n % 420)];
}
