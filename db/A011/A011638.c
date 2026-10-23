/*
 * A011638  63rd cyclotomic polynomial.
 * https://oeis.org/A011638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 63rd cyclotomic polynomial
 *            Φ₆₃(x)
 * Range:     i8 n <= 36, i16 n <= 36, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* degree of Φ₆₃ */
#define DEG63 36
/* maximal degree needed in intermediate calculations */
#define MAX_DEG 63
/* maximum number of coefficients we will handle */
#define MAX_COEFF (MAX_DEG + 1)

/* polynomial data */
static seq_t phi_coeff[64][MAX_COEFF];
static idx_t phi_deg[64] = {0};

/* helper: multiply two polynomials a(degA) and b(degB) into out (degA+degB) */
static void poly_mul(const seq_t *a, idx_t degA,
                     const seq_t *b, idx_t degB,
                     seq_t *out)
{
    for (idx_t i = 0; i <= degA + degB; ++i)
        out[i] = 0;
    for (idx_t i = 0; i <= degA; ++i)
        for (idx_t j = 0; j <= degB; ++j)
            out[i + j] += a[i] * b[j];
}

/* helper: divide polynomial P(degP) by monic divisor Q(degQ) into quotient (degP-degQ) */
/* Note: the remainder is discarded (it is zero by construction). */
static void poly_div(const seq_t *P, idx_t degP,
                     const seq_t *Q, idx_t degQ,
                     seq_t *quot)
{
    /* copy dividend to a working buffer */
    seq_t rem[MAX_COEFF];
    for (idx_t i = 0; i <= degP; ++i)
        rem[i] = P[i];

    idx_t q_deg = degP - degQ;
    for (idx_t i = 0; i <= q_deg; ++i)
        quot[i] = 0;

    for (idx_t i = degP; i >= degQ; --i) {
        seq_t coef = rem[i];                 /* leading coefficient, divisor is monic */
        idx_t pos = i - degQ;
        quot[pos] = coef;
        for (idx_t j = 0; j <= degQ; ++j)
            rem[pos + j] -= coef * Q[j];
    }
}

/* compute all cyclotomic polynomials Φₙ(x) for 1 ≤ n ≤ 63 */
static void compute_all_phi(void)
{
    /* Φ₁(x) = x - 1 */
    phi_coeff[1][0] = (seq_t)(-1);
    phi_coeff[1][1] = (seq_t)(1);
    phi_deg[1] = 1;

    for (idx_t n = 2; n <= 63; ++n) {
        /* product of Φ_d for all proper divisors d of n */
        seq_t prod[MAX_COEFF] = {0};
        prod[0] = 1;                  /* start with constant polynomial 1 */
        idx_t deg_prod = 0;

        /* multiply by each Φ_d where d | n and d < n */
        for (idx_t d = 1; d < n; ++d) {
            if (n % d == 0) {
                seq_t temp[MAX_COEFF];
                poly_mul(prod, deg_prod,
                         phi_coeff[d], phi_deg[d],
                         temp);
                deg_prod += phi_deg[d];
                for (idx_t i = 0; i <= deg_prod; ++i)
                    prod[i] = temp[i];
            }
        }

        /* dividend: x^n - 1 */
        seq_t divd[MAX_COEFF];
        for (idx_t i = 0; i <= n; ++i)
            divd[i] = 0;
        divd[0] = (seq_t)(-1);
        divd[n] = (seq_t)(1);

        /* division: (x^n - 1) / prod = Φ_n(x) */
        seq_t quot[MAX_COEFF] = {0};
        poly_div(divd, n, prod, deg_prod, quot);
        idx_t deg_n = n - deg_prod;

        /* store result */
        phi_deg[n] = deg_n;
        for (idx_t i = 0; i <= deg_n; ++i)
            phi_coeff[n][i] = quot[i];
    }
}

/* public function required by the build */
seq_t a(idx_t n)
{
    /* lazily compute all needed polynomials once */
    static bool initialized = false;
    if (!initialized) {
        compute_all_phi();
        initialized = true;
    }

    if (n > DEG63)
        return (seq_t)0;                 /* outside the degree of Φ₆₃ */
    return phi_coeff[63][(size_t)n];
}
