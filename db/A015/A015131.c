/*
 * A015131  Inverse of 1122nd cyclotomic polynomial.
 * https://oeis.org/A015131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the periodic sequence a(n) = coefficient of x^n in the formal
 * power series 1/Φ_1122(x), where Φ_1122 is the 1122nd cyclotomic
 * polynomial.  The sequence repeats with period 1122. */

#include <stddef.h>
#include <stdbool.h>

#define MAX_DIVISORS 32
#define MAX_D 1122
/* Φ_1122 has degree φ(1122)=320 */
#define MAX_PHI_DEG 320

static bool initialized = false;

/* Global storage for the Φ_1122 coefficients and the period */
static seq_t phi_coef[MAX_PHI_DEG + 1];
static int phi_deg;
static seq_t period_arr[1122];

/* Helper to multiply two polynomials A (degA) and B (degB).  Result in C. */
static void poly_mul(const seq_t *A, int degA,
                     const seq_t *B, int degB,
                     seq_t *C)
{
    /* C must have space for degA+degB+1 coeffs */
    int total_deg = degA + degB;
    for (int i = 0; i <= total_deg; ++i) C[i] = 0;
    for (int i = 0; i <= degA; ++i) {
        seq_t ai = A[i];
        if (!ai) continue;
        for (int j = 0; j <= degB; ++j) {
            C[i + j] += ai * B[j];
        }
    }
}

/* Helper to divide dividend (degD) by divisor (degS), both monic.
 * Dividend is overwritten with the quotient (degree = degD - degS). */
static void poly_div(const seq_t *dividend, int degD,
                     const seq_t *divisor, int degS,
                     seq_t *quotient)
{
    seq_t rem[MAX_D + 1];
    for (int i = 0; i <= degD; ++i) rem[i] = dividend[i];
    int qdeg = degD - degS;
    for (int i = 0; i <= qdeg; ++i) quotient[i] = 0;
    for (int i = degD; i >= degS; --i) {
        seq_t coeff = rem[i];
        if (!coeff) continue;
        int shift = i - degS;
        quotient[shift] = coeff;
        for (int j = 0; j <= degS; ++j) {
            rem[shift + j] -= coeff * divisor[j];
        }
    }
}

/* Initialise the Φ_1122 coefficients and the period array */
static void init_sequence(void)
{
    /* Compute all divisors of 1122 */
    int divs[MAX_DIVISORS];
    int num_div = 0;
    for (int d = 1; d <= 1122; ++d) {
        if (1122 % d == 0) divs[num_div++] = d;
    }

    /* Storage for all Φ_d polynomials */
    static seq_t poly[MAX_DIVISORS][MAX_D + 1];
    static int deg[MAX_DIVISORS];

    /* Zero initialise */
    for (int i = 0; i < num_div; ++i) {
        deg[i] = 0;
        for (int j = 0; j <= MAX_D; ++j) poly[i][j] = 0;
    }

    /* Compute Φ_d for each divisor in increasing order */
    for (int idx = 0; idx < num_div; ++idx) {
        int d = divs[idx];
        if (d == 1) {
            /* Φ_1(x) = x - 1 */
            deg[0] = 1;
            poly[0][0] = -1;
            poly[0][1] = 1;
            continue;
        }

        /* Build product of all Φ_e for e|d, e<d */
        static seq_t prod[MAX_D + 1];
        static seq_t tmp[MAX_D + 1];
        for (int i = 0; i <= MAX_D; ++i) prod[i] = 0;
        prod[0] = 1;
        int prod_deg = 0;

        for (int eidx = 0; eidx < idx; ++eidx) {
            int e = divs[eidx];
            if (d % e != 0) continue;
            int e_deg = deg[eidx];
            /* tmp = prod * Φ_e */
            int new_deg = prod_deg + e_deg;
            for (int i = 0; i <= new_deg; ++i) tmp[i] = 0;
            for (int i = 0; i <= prod_deg; ++i) {
                seq_t pi = prod[i];
                if (!pi) continue;
                for (int j = 0; j <= e_deg; ++j) {
                    tmp[i + j] += pi * poly[eidx][j];
                }
            }
            for (int i = 0; i <= new_deg; ++i) prod[i] = tmp[i];
            prod_deg = new_deg;
        }

        /* Dividend = x^d - 1 */
        static seq_t divd[MAX_D + 1];
        for (int i = 0; i <= d; ++i) divd[i] = 0;
        divd[0] = -1;
        divd[d] = 1;

        /* Dividing dividend by product */
        static seq_t quot[MAX_D + 1];
        poly_div(divd, d, prod, prod_deg, quot);
        deg[idx] = d - prod_deg;

        for (int i = 0; i <= deg[idx]; ++i) poly[idx][i] = quot[i];
        for (int i = deg[idx] + 1; i <= MAX_D; ++i) poly[idx][i] = 0;
    }

    /* Find index of d=1122 */
    int idx1122 = -1;
    for (int i = 0; i < num_div; ++i) {
        if (divs[i] == 1122) {
            idx1122 = i;
            break;
        }
    }

    /* Copy the Φ_1122 coefficients */
    phi_deg = deg[idx1122];
    for (int i = 0; i <= phi_deg; ++i) {
        phi_coef[i] = poly[idx1122][i];
    }

    /* Compute the period array (period length = 1122) */
    period_arr[0] = 1;
    for (int n = 1; n < 1122; ++n) {
        seq_t sum = 0;
        int limit = (n <= phi_deg) ? n : phi_deg;
        for (int k = 1; k <= limit; ++k) {
            sum += phi_coef[k] * period_arr[n - k];
        }
        period_arr[n] = -sum;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    idx_t m = n % 1122;
    return period_arr[m];
}
