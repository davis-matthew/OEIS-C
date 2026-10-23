/*
 * A016378  308th cyclotomic polynomial.
 * https://oeis.org/A016378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 308th cyclotomic polynomial
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Φ_308(x) = (x^308-1) / ∏_{d|308, d<308} Φ_d(x).  The implementation   */
/* builds all Φ_d for the 12 divisors of 308 by successive polynomial      */
/* divisions, so all intermediate values stay within 64‑bit bounds.         */

#include <stddef.h>

/* Number of divisors of 308 and maximum number of coefficients */
enum { DIV_COUNT = 12 };
enum { MAX_DEG   = 309 };

/* Sorted list of the 12 divisors of 308 */
static const idx_t divs[DIV_COUNT] = {
    1, 2, 4, 7, 11, 14, 22, 28, 44, 77, 154, 308
};

/* Storage for Φ_d coefficients; coeff[k] = coefficient of x^k */
static seq_t phi[DIV_COUNT][MAX_DEG];
/* Number of coefficients in Φ_d (i.e., degree + 1) */
static int  deg_phi[DIV_COUNT];

/* Polynomial division:  A / B  (both monic)
   A has na coefficients (indices 0..na-1),
   B has nb coefficients (indices 0..nb-1).
   Resulting quotient is stored in q, with nq = number of coeffs. */
static void poly_div(const seq_t *a, int na,
                     const seq_t *b, int nb,
                     seq_t *q, int *nq)
{
    int degA = na - 1;
    int degB = nb - 1;
    seq_t tmp[MAX_DEG];

    /* Copy dividend into temporary buffer (only first na entries are needed) */
    for (int i = 0; i < na; ++i)
        tmp[i] = a[i];

    /* Prepare quotient array */
    int qdeg = degA - degB;
    for (int i = 0; i <= qdeg; ++i)
        q[i] = 0;

    /* Long division */
    for (int k = degA; k >= degB; --k) {
        seq_t coeff = tmp[k];
        q[k - degB] = coeff;
        if (coeff != 0) {
            /* Subtract (coeff * b) shifted to align with degree k */
            for (int j = 0; j < degB; ++j)
                tmp[k - degB + j] -= coeff * b[j];
        }
    }
    *nq = qdeg + 1;
}

/* Build all Φ_d for d | 308 */
static void init_phi_308(void)
{
    static int prepared = 0;
    if (prepared) return;
    prepared = 1;

    for (int i = 0; i < DIV_COUNT; ++i) {
        idx_t d = divs[i];

        if (i == 0) {                /* Φ_1(x) = x - 1 */
            deg_phi[0] = 2;
            phi[0][0] = -1;
            phi[0][1] =  1;
            for (int k = 2; k < MAX_DEG; ++k) phi[0][k] = 0;
            continue;
        }

        /* Numerator: x^d - 1 */
        seq_t temp[MAX_DEG];
        int len = d + 1;             /* number of coefficients in numerator */
        for (int k = 0; k < len; ++k) temp[k] = 0;
        temp[0] = -1;
        temp[d] =  1;

        /* Successively divide by all Φ_e with e < d */
        for (int j = 0; j < i; ++j) {
            if ((d % divs[j]) == 0u) {
                int lenB = deg_phi[j];   /* φ_e has 'lenB' coefficients */
                /* In-place division: quotient overwrites temp */
                poly_div(temp, len, phi[j], lenB, temp, &len);
            }
        }

        /* Store the result Φ_d */
        deg_phi[i] = len;
        for (int k = 0; k < len; ++k) phi[i][k] = temp[k];
        for (int k = len; k < MAX_DEG; ++k) phi[i][k] = 0;
    }
}

/* Public function required by the template */
seq_t a(idx_t n)
{
    init_phi_308();                     /* ensure Φ_308 is computed */
    const int IDX_308 = 11;             /* divs[11] == 308 */
    if (n < (idx_t)deg_phi[IDX_308])    /* only valid indices return values */
        return phi[IDX_308][(int)n];
    return 0;                           /* coefficients beyond degree are zero */
}
