/*
 * A015335  Inverse of 1326th cyclotomic polynomial.
 * https://oeis.org/A015335
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

/*  a(n) is the coefficient of x^n in the power series expansion of 1/Φ_1326(x)  */
/*  where Φ_1326 is the 1326th cyclotomic polynomial.  The sequence is periodic
    with period 1326 (A015335).  This implementation computes the first
    1326 terms once and returns the value for any n using the period.  */

#include <stddef.h>

#define MAX_DEG 1326          /* highest degree that can occur in intermediate polys */
#define MAX_COEFF 385         /* φ(1326)+1  = 384+1  */
#define NUM_DIV 16            /* number of divisors of 1326 */

/* polynomial representation */
typedef struct {
    int deg;                     /* degree of the polynomial */
    seq_t coeff[MAX_DEG + 1];    /* coeff[i] = coefficient of x^i, 0 <= i <= deg */
} Poly;

/* set polynomial to zero */
static void poly_set_zero(Poly *p) {
    p->deg = 0;
    p->coeff[0] = 0;
}

/* set polynomial to one */
static void poly_set_one(Poly *p) {
    p->deg = 0;
    p->coeff[0] = 1;
}

/* set polynomial to x^k (i.e. coefficient 1 at degree k) */
static void poly_set_xpow(Poly *p, int k) {
    int i;
    p->deg = k;
    for (i = 0; i <= k; ++i) p->coeff[i] = 0;
    p->coeff[k] = 1;
}

/* copy polynomial src into dst */
static void poly_copy(const Poly *src, Poly *dst) {
    int i;
    dst->deg = src->deg;
    for (i = 0; i <= src->deg; ++i) dst->coeff[i] = src->coeff[i];
}

/* polynomial multiplication (exact) */
static void poly_mul(const Poly *a, const Poly *b, Poly *res) {
    int i, j, k;
    int newdeg = a->deg + b->deg;
    for (k = 0; k <= newdeg; ++k) res->coeff[k] = 0;
    for (i = 0; i <= a->deg; ++i) {
        seq_t ai = a->coeff[i];
        if (ai == 0) continue;
        for (j = 0; j <= b->deg; ++j) {
            res->coeff[i + j] += ai * b->coeff[j];
        }
    }
    res->deg = newdeg;
}

/* exact polynomial division: res = num / den  (leading coefficient of den is 1) */
static void poly_div_exact(const Poly *num, const Poly *den, Poly *res) {
    int ndeg = num->deg;
    int ddeg = den->deg;
    int rdeg = ndeg - ddeg;
    seq_t rem[MAX_DEG + 1] = {0};
    int i, k;

    for (i = 0; i <= ndeg; ++i) rem[i] = num->coeff[i];
    /* initialize quotient */
    for (i = 0; i <= rdeg; ++i) res->coeff[i] = 0;

    for (k = rdeg; k >= 0; --k) {
        seq_t coeff = rem[ddeg + k];
        if (coeff != 0) {
            res->coeff[k] = coeff;
            for (i = 0; i <= ddeg; ++i) {
                rem[i + k] -= coeff * den->coeff[i];
            }
        }
    }
    res->deg = rdeg;
}

/* main sequence function */
seq_t a(idx_t n) {
    static int initialized = 0;
    static seq_t coeff[MAX_COEFF];
    static seq_t cache[1326];
    if (!initialized) {
        /* list of divisors of 1326 in ascending order */
        const int divs[NUM_DIV] = {1,2,3,6,13,17,26,34,39,51,78,102,221,442,663,1326};

        Poly phi[NUM_DIV];
        int i, j, d;
        /* compute all cyclotomic polynomials Φ_d for d | 1326 */
        for (i = 0; i < NUM_DIV; ++i) {
            d = divs[i];

            /* denominator = product of Φ_e for e < d that divide d */
            Poly denom;
            poly_set_one(&denom);
            for (j = 0; j < i; ++j) {
                if (d % divs[j] == 0) {
                    Poly tmp;
                    poly_mul(&denom, &phi[j], &tmp);
                    poly_copy(&tmp, &denom);
                }
            }

            /* numerator = x^d - 1 */
            Poly num;
            poly_set_zero(&num);
            num.deg = d;
            /* ensure zeroes up to d */
            for (j = 0; j <= d; ++j) num.coeff[j] = 0;
            num.coeff[0] = -1;
            num.coeff[d] = 1;

            /* Φ_d = numerator / denominator */
            poly_div_exact(&num, &denom, &phi[i]);
        }

        /* locate Φ_1326 */
        int idx_1326 = -1;
        for (i = 0; i < NUM_DIV; ++i) {
            if (divs[i] == 1326) { idx_1326 = i; break; }
        }
        const Poly *phi1326 = &phi[idx_1326];
        int deg = phi1326->deg;
        for (i = 0; i <= deg; ++i) coeff[i] = phi1326->coeff[i];

        /* compute periodic sequence via recurrence a_k + Σ c_i * a_{k-i} = 0 */
        cache[0] = 1;
        for (i = 1; i < 1326; ++i) {
            seq_t sum = 0;
            int limit = (i < deg) ? i : deg;
            int ii;
            for (ii = 1; ii <= limit; ++ii) {
                sum += coeff[ii] * cache[i - ii];
            }
            cache[i] = -sum;
        }
        initialized = 1;
    }
    idx_t pos = n % 1326;
    return cache[pos];
}
