/*
 * A014195  Inverse of 186th cyclotomic polynomial.
 * https://oeis.org/A014195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; for n > 0 a(n) = - Σ_{k=1}^{deg Φ_186} c_k · a(n - k)
 *            where c_k are the coefficients of the cyclotomic polynomial
 *            Φ_186(x). The sequence is periodic with period 186
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_proper_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_proper_divisors.h"

#define PERIOD 186
#define MAX_DEG 200  /* safe upper bound for polynomial degrees */

/* polynomial structure: coeff[i] is the coefficient of x^i */
typedef struct {
    seq_t coeff[MAX_DEG];
    int   deg;      /* actual degree (highest non‑zero exponent) */
} Poly;

static Poly phi_cache[PERIOD + 1];
static bool phi_done[PERIOD + 1] = { false };

/* ------------------------------------------------------------------ */
/* Helpers for polynomial arithmetic                                   */
/* ------------------------------------------------------------------ */

static void poly_zero(Poly *p) {
    for (int i = 0; i < MAX_DEG; ++i) p->coeff[i] = 0;
    p->deg = 0;
}

static void poly_copy(const Poly *src, Poly *dst) {
    dst->deg = src->deg;
    for (int i = 0; i <= src->deg; ++i) dst->coeff[i] = src->coeff[i];
}

static void poly_mul_inplace(Poly *R, const Poly *B) {
    /* R = R * B ;  R and B deg <= MAX_DEG-1  */
    Poly temp;
    poly_zero(&temp);
    for (int i = 0; i <= R->deg; ++i) {
        if (R->coeff[i] == 0) continue;
        for (int j = 0; j <= B->deg; ++j) {
            temp.coeff[i + j] += R->coeff[i] * B->coeff[j];
        }
    }
    temp.deg = R->deg + B->deg;
    while (temp.deg > 0 && temp.coeff[temp.deg] == 0) temp.deg--;
    poly_copy(&temp, R);
}

/* ------------------------------------------------------------------ */
/* Compute all proper divisors of n (excluding n)                     */

/* ------------------------------------------------------------------ */
/* Recursive computation of cyclotomic polynomial Φ_n(x)             */
/* ------------------------------------------------------------------ */
static void compute_phi(idx_t n) {
    if (phi_done[n]) return;

    if (n == 1) {          /* Φ_1(x) = x - 1 */
        poly_zero(&phi_cache[n]);
        phi_cache[n].coeff[0] = -1;
        phi_cache[n].coeff[1] = 1;
        phi_cache[n].deg = 1;
        phi_done[n] = true;
        return;
    }

    /* compute all proper divisors first */
    idx_t divs[32];
    int   dcnt;
    get_proper_divisors(n, divs, &dcnt);
    for (int i = 0; i < dcnt; ++i) compute_phi(divs[i]);

    /* numerator: x^n - 1 */
    Poly numer;
    poly_zero(&numer);
    numer.coeff[0] = -1;
    numer.coeff[n] = 1;
    numer.deg = n;

    /* denominator: product of Φ_d for all proper divisors d */
    Poly denom;
    poly_zero(&denom);
    denom.coeff[0] = 1;   /* constant polynomial 1 */
    denom.deg = 0;
    for (int i = 0; i < dcnt; ++i) poly_mul_inplace(&denom, &phi_cache[divs[i]]);

    /* division: numer / denom  (denom is monic) */
    seq_t Acoeff[MAX_DEG];
    for (int i = 0; i <= numer.deg; ++i) Acoeff[i] = numer.coeff[i];

    Poly quot;
    poly_zero(&quot);
    int degA = numer.deg;
    int degB = denom.deg;
    int degQ = degA - degB;
    quot.deg = degQ;

    for (int i = degA; i >= degB; --i) {
        seq_t coeff = Acoeff[i];                 /* since denom leading coeff = 1 */
        quot.coeff[i - degB] = coeff;
        for (int j = 0; j <= degB; ++j) {
            Acoeff[i - degB + j] -= coeff * denom.coeff[j];
        }
    }
    /* trim trailing zeros */
    while (quot.deg > 0 && quot.coeff[quot.deg] == 0) quot.deg--;

    poly_copy(&quot, &phi_cache[n]);
    phi_done[n] = true;
}

/* ------------------------------------------------------------------ */
/* Periodic coefficients of the sequence a(n)                         */
/* ------------------------------------------------------------------ */
static seq_t period[PERIOD];
static bool period_done = false;

static void compute_period(void) {
    if (period_done) return;
    compute_phi(PERIOD);
    Poly *phi = &phi_cache[PERIOD];
    int d = phi->deg;          /* d = φ(186) = 60 */

    period[0] = (seq_t)1;
    for (int n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        int maxk = (n < d) ? n : d;
        for (int k = 1; k <= maxk; ++k)
            sum += phi->coeff[k] * period[n - k];
        period[n] = -sum;
    }
    period_done = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    compute_period();
    return period[(int)(n % PERIOD)];
}
