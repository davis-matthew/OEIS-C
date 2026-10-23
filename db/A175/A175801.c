/*
 * A175801  Number of real zeros of the polynomial whose coefficients are the
 *          decimal digits of prime(n).
 * https://oeis.org/A175801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   get_prime, sign_ld
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n): the number of real zeros of the polynomial
 * whose coefficients are the decimal digits of the n-th prime.
 * The algorithm uses Sturm's theorem with long double arithmetic.
 */
#include <stdbool.h>
#include <math.h>
#include "../utils/get_prime.h"
#include "../utils/sign_ld.h"

#define MAX_DIGITS 6    /* 104729 has 6 digits, prime(10000) */
#define MAX_POLY  10   /* maximum number of polynomials in Sturm sequence */

typedef struct {
    int deg;                    /* degree of polynomial (deg < 0 means zero polynomial) */
    long double coeff[MAX_DIGITS]; /* coefficients in increasing order: coeff[0] + coeff[1]*x + ... */
} Poly;

/* derivative: P' */
static Poly poly_derivative(const Poly *p) {
    Poly d = { .deg = p->deg - 1, .coeff = {0} };
    if (p->deg < 0) {
        d.deg = -1;
        return d;
    }
    for (int i = 1; i <= p->deg; ++i)
        d.coeff[i - 1] = p->coeff[i] * i;
    return d;
}

/* copy polynomial */
static Poly poly_copy(const Poly *p) {
    Poly r = { .deg = p->deg, .coeff = {0} };
    for (int i = 0; i <= p->deg; ++i)
        r.coeff[i] = p->coeff[i];
    return r;
}

/* remainder of P divided by Q, Q.leading != 0 */
static Poly poly_remainder(const Poly *P, const Poly *Q) {
    const long double eps = 1e-18L;
    Poly R = poly_copy(P);
    int degP = R.deg;
    int degQ = Q->deg;
    if (degQ < 0) {
        R.deg = -1;  /* division by zero polynomial not expected */
        return R;
    }
    while (degP >= degQ && degP >= 0) {
        long double factor = R.coeff[degP] / Q->coeff[degQ];
        int shift = degP - degQ;
        for (int i = 0; i <= degQ; ++i)
            R.coeff[shift + i] -= factor * Q->coeff[i];
        while (degP >= 0 && fabsl(R.coeff[degP]) < eps)
            degP--;
    }
    R.deg = degP;
    return R;
}

/* sign of polynomial at infinity: true for +infinity, false for -infinity */
static int sign_at_infinity(const Poly *p, bool positive_infinity) {
    if (p->deg < 0) return 0;  /* zero polynomial never evaluated */
    int s = sign_ld(p->coeff[p->deg]);  /* leading coefficient sign */
    if (!positive_infinity && (p->deg & 1)) s = -s;
    return s;
}

/* count sign variations at infinity over the Sturm sequence */
static int sturm_variations_at_infinity(const Poly seq[], int len, bool positive_infinity) {
    int last = 0;
    int var = 0;
    for (int i = 0; i < len; ++i) {
        if (seq[i].deg < 0) continue;
        int s = sign_at_infinity(&seq[i], positive_infinity);
        if (last == 0) {
            last = s;
        } else if (s != 0 && s != last) {
            ++var;
            last = s;
        }
    }
    return var;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* obtain the n-th prime (1-indexed) */
    uint32_t prime = get_prime(n);

    /* build polynomial from digits (least significant first) */
    Poly f = { .deg = -1, .coeff = {0} };
    int pos = 0;
    uint32_t temp = prime;
    while (temp > 0) {
        f.coeff[pos++] = temp % 10;
        temp /= 10;
    }
    f.deg = pos - 1;
    if (f.deg < 0) {           /* prime 0? impossible but safe */
        return 0;
    }

    /* Sturm sequence construction */
    Poly sturm[MAX_POLY];
    int m = 0;
    sturm[m++] = f;
    sturm[m++] = poly_derivative(&f);

    while (m >= 2 && sturm[m - 1].deg >= 0) {
        Poly rem = poly_remainder(&sturm[m - 2], &sturm[m - 1]);
        /* multiply remainder by -1 to follow Sturm definition */
        for (int i = 0; i <= rem.deg; ++i)
            rem.coeff[i] = -rem.coeff[i];
        if (rem.deg < 0) break;
        if (m >= MAX_POLY) break;  /* safety */
        sturm[m++] = rem;
    }

    int var_minus = sturm_variations_at_infinity(sturm, m, false);
    int var_plus  = sturm_variations_at_infinity(sturm, m, true);
    int roots = var_minus - var_plus;
    return (seq_t)roots;
}
