/*
 * A015643  Inverse of 1634th cyclotomic polynomial.
 * https://oeis.org/A015643
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

/* 
   Compute a(n) = coefficient of x^n in the inverse power series of the 1634th
   cyclotomic polynomial Φ_1634(x).  The sequence is periodic with period
   1634, so a(n) = a(n mod 1634).  The coefficients are obtained by solving
   the convolution equation Φ_1634(x) * A(x) = 1 with A(0)=1.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Maximum degree we ever need: Φ_1634 has degree 756, the product of all
   proper cyclotomic polynomials has degree 878, and the dividend x^1634-1 has
   degree 1634.  So 1635 is a safe upper bound for all intermediates. */
#define MAX_DEG 1635

/* Helper structure for a polynomial with degree 'deg' and coefficient array 'c'.
   The array has length deg+1, with c[0] the constant term. */
typedef struct {
    seq_t *c;          /* coefficient array on the heap */
    size_t deg;        /* degree of the polynomial */
} Poly;

/* Allocate a polynomial of given degree; all coefficients are zero */
static Poly *poly_new(size_t deg)
{
    Poly *p = (Poly *)malloc(sizeof(Poly));
    if (!p) return NULL;
    p->c = (seq_t *)calloc(deg + 1, sizeof(seq_t));
    p->deg = deg;
    return p;
}

/* Free a polynomial */
static void poly_free(Poly *p)
{
    if (!p) return;
    free(p->c);
    free(p);
}

/* Multiply two polynomials.  Returns a new polynomial. */
static Poly *poly_mul(const Poly *a, const Poly *b)
{
    size_t da = a->deg, db = b->deg;
    size_t dc = da + db;
    Poly *c = poly_new(dc);
    for (size_t i = 0; i <= da; ++i) {
        if (a->c[i] == 0) continue;
        for (size_t j = 0; j <= db; ++j) {
            if (b->c[j] == 0) continue;
            c->c[i + j] += a->c[i] * b->c[j];
        }
    }
    return c;
}

/* Divide polynomial A by monic polynomial B (B's leading coefficient is 1).
   The division is exact.  Returns the quotient Q.  Both arguments are
   non-const because we modify the remainder vector. */
static Poly *poly_div_monic(const Poly *A, const Poly *B)
{
    size_t da = A->deg, db = B->deg;
    if (da < db) {
        /* quotient 0, not needed for our usage */
        Poly *q = poly_new(0);
        return q;
    }
    size_t dq = da - db;
    Poly *Q = poly_new(dq);
    /* Make a mutable copy of the dividend */
    Poly *R = poly_new(da);
    for (size_t i = 0; i <= da; ++i) R->c[i] = A->c[i];

    for (int k = (int)dq; k >= 0; --k) {
        seq_t coeff = R->c[db + k] / B->c[db]; /* B->c[db] == 1 */
        if (coeff != 0) {
            Q->c[k] = coeff;
            /* Subtract coeff * x^k * B from R */
            for (size_t j = 0; j <= db; ++j) {
                R->c[k + j] -= coeff * B->c[j];
            }
        }
    }
    poly_free(R);
    return Q;
}

/* List of all divisors of 1634 in ascending order */
static const idx_t DIVISORS[] = {1, 2, 19, 38, 43, 86, 817, 1634};
static const size_t NDIV = sizeof(DIVISORS)/sizeof(DIVISORS[0]);

/* Compute all cyclotomic polynomials Φ_d for d in DIVISORS.  The results
   are stored in the array 'phi_out', which must have NDIV elements
   pre-allocated.  Each element points to a newly allocated Poly. */
static void compute_all_phi(Poly *phi_out[NDIV])
{
    for (size_t idx = 0; idx < NDIV; ++idx) {
        idx_t d = DIVISORS[idx];
        /* Build dividend x^d - 1 */
        Poly *dividend = poly_new(d);
        dividend->c[0] = -1;
        dividend->c[d] = 1;
        /* Build product of all proper cyclotomic polynomials of d */
        Poly *prod = poly_new(0);          /* 1 */
        prod->c[0] = 1;
        for (size_t j = 0; j < idx; ++j) { /* all previous divisors are < d */
            if (d % DIVISORS[j] != 0) continue;
            Poly *tmp = poly_mul(prod, phi_out[j]);
            poly_free(prod);
            prod = tmp;
        }
        /* Compute Φ_d = dividend / prod */
        Poly *phi = poly_div_monic(dividend, prod);
        poly_free(dividend);
        poly_free(prod);
        phi_out[idx] = phi;
    }
}

/* Compute the first 1634 coefficients of the inverse power series of Φ_1634.
   The coefficients are stored in the static array 'period'. */
static void compute_period_coefficients(seq_t period[1634])
{
    /* φ(1634) is the last entry */
    const idx_t target = 1634;
    Poly *phi_out[NDIV];
    compute_all_phi(phi_out);

    Poly *phi_1634 = NULL;
    for (size_t i = 0; i < NDIV; ++i)
        if (DIVISORS[i] == target) { phi_1634 = phi_out[i]; break; }

    size_t dphi = phi_1634->deg;             /* degree = φ(1634) = 756 */
    seq_t *phi_coeff = phi_1634->c;          /* Φ_1634 coefficients, φ_1634[0]=1 */

    /* Now compute a[0..1633] using the recurrence */
    period[0] = 1;
    for (size_t n = 1; n < 1634; ++n) {
        seq_t sum = 0;
        size_t kmax = (n < dphi) ? n : dphi;
        for (size_t k = 1; k <= kmax; ++k) {
            sum += phi_coeff[k] * period[n - k];
        }
        period[n] = -sum;
    }

    /* Free allocated polynomials */
    for (size_t i = 0; i < NDIV; ++i)
        poly_free(phi_out[i]);
}

/* Main function requested by the problem statement */
seq_t a(idx_t n)
{
    /* static table holds the periodic coefficients */
    static seq_t period[1634];
    static bool initialized = false;
    if (!initialized) {
        compute_period_coefficients(period);
        initialized = true;
    }
    idx_t idx = n % 1634;
    return period[idx];
}
