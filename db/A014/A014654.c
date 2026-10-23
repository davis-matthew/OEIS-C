/*
 * A014654  Inverse of 645th cyclotomic polynomial.
 * https://oeis.org/A014654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ₆₄₅(x), the inverse of the
 *            645‑th cyclotomic polynomial
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* --- Polynomial type (degree ≤ 645) ------------------------------------ */
typedef struct {
    int deg;              /* actual degree (≤ 645)                 */
    seq_t coeff[700];     /* coefficient array, coeff[0] is constant term */
} Poly;

/* --- Generic helper routines ------------------------------------------ */

/* Remove all coefficients of a polynomial. */
static void poly_clear(Poly *p) {
    p->deg = 0;
    for (int i = 0; i < 700; ++i) p->coeff[i] = 0;
}

/* Multiply two polynomials: R = A * B.  (All degrees ≤ 645).            */
static void poly_mul(const Poly *A, const Poly *B, Poly *R) {
    int dA = A->deg, dB = B->deg;
    int dR = dA + dB;
    for (int i = 0; i <= dR; ++i) R->coeff[i] = 0;
    for (int i = 0; i <= dA; ++i) {
        if (!A->coeff[i]) continue;
        for (int j = 0; j <= dB; ++j) {
            if (!B->coeff[j]) continue;
            R->coeff[i + j] += A->coeff[i] * B->coeff[j];
        }
    }
    R->deg = dR;
}

/* Divide polynomial Q by the monic polynomial P: Q = P * R.  (Q is
   unchanged).  The result R is placed into the supplied argument.      */
static void poly_div(const Poly *Q, const Poly *P, Poly *R) {
    int dQ = Q->deg, dP = P->deg;
    int dR = dQ - dP;
    seq_t temp[700];
    for (int i = 0; i <= dQ; ++i) temp[i] = Q->coeff[i];
    for (int i = 0; i <= dR; ++i) R->coeff[i] = 0;

    for (int i = dQ; i >= dP; --i) {
        seq_t coef = temp[i];
        if (!coef) continue;
        int shift = i - dP;
        R->coeff[shift] = coef;                 /* leading coefficient is 1 */
        for (int j = 0; j <= dP; ++j) {
            temp[shift + j] -= coef * P->coeff[j];
        }
    }
    R->deg = dR;
}

/* Compute all proper divisors of n (n > 1).  The array 'divs' must
   have capacity at least 64.  The function stores the count in 'cnt'. */
static void get_proper_divisors(idx_t n, idx_t *divs, int *cnt) {
    int c = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (d != n) { divs[c++] = d; }
            idx_t e = n / d;
            if (e != d && e != n) { divs[c++] = e; }
        }
    }
    *cnt = c;
}

/* --------------------------------------------------------------------- */

/* Cache for cyclotomic polynomials Φ_n(x) (0 ≤ n ≤ 645).               */
static Poly cache[650];
static bool computed[650] = {0};

/* Forward declaration. */
static void compute_phi(int n);

/* Recursively compute Φ_n(x) and store it in cache[n].                */
static void compute_phi(int n) {
    if (computed[n]) return;

    if (n == 1) {
        /* Φ₁(x) = x - 1. */
        poly_clear(&cache[n]);
        cache[n].deg = 1;
        cache[n].coeff[0] = -1;
        cache[n].coeff[1] = 1;
    } else {
        idx_t divs[64];
        int div_cnt;
        get_proper_divisors((idx_t)n, divs, &div_cnt);

        /* Product of Φ_d for all proper divisors d of n. */
        Poly prod, tmp;
        poly_clear(&prod);
        prod.deg = 0;
        prod.coeff[0] = 1;                 /* multiplicative identity */

        for (int i = 0; i < div_cnt; ++i) {
            int d = (int)divs[i];
            compute_phi(d);
            poly_mul(&prod, &cache[d], &tmp);
            prod = tmp;                    /* copy result */
        }

        /* Q(x) = x^n - 1. */
        Poly Q;
        poly_clear(&Q);
        Q.deg = n;
        Q.coeff[0] = -1;
        Q.coeff[n] = 1;

        /* Φ_n(x) = Q(x) / prod(x).   The divisor is monic. */
        poly_div(&Q, &prod, &cache[n]);
    }

    computed[n] = true;
}

/* Periodic array of the sequence a(n).  Period is 645. */
#define PERIOD 645
static seq_t period[PERIOD];
static bool period_initialized = false;

/* Initialise the period array.  The routine guarantees that
   compute_phi(645) has been executed. */
static void init_period(void) {
    compute_phi(645);
    Poly *phi = &cache[645];
    int deg_phi = phi->deg;

    period[0] = 1;                                /* a(0) = 1 */
    for (int k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        int imax = k < deg_phi ? k : deg_phi;
        for (int i = 1; i <= imax; ++i) {
            sum += phi->coeff[i] * period[k - i];
        }
        period[k] = -sum;
    }
    period_initialized = true;
}

/* --------------------------------------------------------------------- */
/* Public function required by the build.                                 */

seq_t a(idx_t n)
{
    if (!period_initialized) init_period();
    idx_t idx = n % PERIOD;
    return period[idx];
}
