/*
 * A015824  Inverse of 1815th cyclotomic polynomial.
 * https://oeis.org/A015824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1/Φ_1815(x), where Φ_1815 is the 1815th cyclotomic polynomial.
 *            The sequence is periodic with period 1815
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required for bool type */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* DIVISORS of 1815: 3 * 5 * 11^2 */
static const idx_t DIVISORS[12] = {
    1, 3, 5, 11,
    15, 33, 55, 121,
    165, 363, 605, 1815
};

/* multiply two polynomials:  a[0..da] * b[0..db] -> result[0..da+db] */
static seq_t *poly_mul(const seq_t *a, idx_t da, const seq_t *b,
                       idx_t db)
{
    idx_t d = da + db;
    seq_t *c = (seq_t *)calloc((size_t)(d + 1), sizeof(seq_t));
    for (idx_t i = 0; i <= da; ++i) {
        for (idx_t j = 0; j <= db; ++j) {
            c[i + j] += a[i] * b[j];
        }
    }
    return c;
}

/* divide polynomial numer[0..dn] by denom[0..dd] with dd <= dn
   (division is exact). Return quotient[0..dq] where dq=dn-dd. */
static seq_t *poly_div(const seq_t *numer, idx_t dn,
                       const seq_t *denom, idx_t dd)
{
    idx_t dq = dn - dd;
    seq_t *qr = (seq_t *)calloc((size_t)(dq + 1), sizeof(seq_t));
    /* working copy of numerator */
    seq_t *wrk = (seq_t *)calloc((size_t)(dn + 1), sizeof(seq_t));
    memcpy(wrk, numer, (size_t)(dn + 1) * sizeof(seq_t));

    for (idx_t i = dn; i >= dd; --i) {
        seq_t coeff = wrk[i];
        qr[i - dd] = coeff;
        for (idx_t j = 0; j <= dd; ++j) {
            wrk[i - dd + j] -= coeff * denom[j];
        }
    }
    free(wrk);
    return qr;
}

/* helper: index of divisor in DIVISORS array */
static int div_index(idx_t d)
{
    for (int i = 0; i < 12; ++i) {
        if (DIVISORS[i] == d) return i;
    }
    return -1;
}

/* compute all Φ_d for d | 1815, store in phi[i] and deg_phi[i] */
static void compute_all_phi(seq_t *phi[], idx_t deg_phi[])
{
    for (int i = 0; i < 12; ++i) {
        idx_t d = DIVISORS[i];
        if (d == 1) {          /* Φ_1(x) = x - 1 */
            phi[i] = (seq_t *)calloc(2, sizeof(seq_t));
            phi[i][0] = (seq_t)-1;
            phi[i][1] = (seq_t)1;
            deg_phi[i] = 1;
            continue;
        }

        /* product of Φ_e for all proper divisors e of d */
        seq_t *prod = (seq_t *)calloc(1, sizeof(seq_t));
        prod[0] = (seq_t)1;
        idx_t deg_prod = 0;

        for (int j = 0; j < i; ++j) {
            if (d % DIVISORS[j] == 0) {
                seq_t *newp = poly_mul(prod, deg_prod,
                                       phi[j], deg_phi[j]);
                free(prod);
                prod = newp;
                deg_prod += deg_phi[j];
            }
        }

        /* numerator x^d - 1 */
        seq_t *num = (seq_t *)calloc((size_t)(d + 1), sizeof(seq_t));
        num[0] = (seq_t)-1;
        num[d]  = (seq_t)1;

        /* quotient is Φ_d */
        seq_t *quo = poly_div(num, d, prod, deg_prod);
        seq_t *phi_d = quo;               /* rename for clarity */
        idx_t deg_phi_d = d - deg_prod;   /* degree of Φ_d */

        /* free temporaries except the result */
        free(prod);
        free(num);

        phi[i] = phi_d;
        deg_phi[i] = deg_phi_d;
    }
}

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t period[1815];

    if (!ready) {
        /* compute Φ_1815 coefficients once */
        static seq_t *phi[12];
        static idx_t deg_phi[12];
        compute_all_phi(phi, deg_phi);

        idx_t deg = deg_phi[div_index(1815)];   /* should be 880 */
        /* a(0) = 1 */
        period[0] = (seq_t)1;
        for (idx_t i = 1; i < 1815; ++i) {
            seq_t sum = (seq_t)0;
            idx_t up = (i < deg) ? i : deg;
            for (idx_t k = 1; k <= up; ++k) {
                sum += phi[div_index(1815)][k] * period[i - k];
            }
            period[i] = -sum;
        }

        /* free temporary φ polynomials (not needed later) */
        for (int i = 0; i < 12; ++i) {
            free(phi[i]);
        }

        ready = true;
    }

    return period[(size_t)(n % 1815)];
}
