/*
 * A014961  Inverse of 952nd cyclotomic polynomial.
 * https://oeis.org/A014961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1 / Φ_952(x). This satisfies a(0) = 1, a(n) =
 *            -∑_{k=1}^{min(n,deg)} φ_k * a[n-k], where φ_k are the
 *            coefficients of the monic cyclotomic polynomial Φ_952(x) (deg =
 *            384)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>   /* for uint64_t etc. */

#define MAX_DIV 16   /* number of divisors of 952 = 2^3·7·17 */

/* Polynomial representation -------------------------------------------- */
typedef struct {
    idx_t deg;           /* degree of the polynomial (>=0) */
    seq_t *coeff;        /* coeff[i] is the coefficient of x^i, 0 <= i <= deg */
} Poly;

/* Allocate a zeroed polynomial of given degree. */
static Poly *poly_new(idx_t deg)
{
    Poly *p = (Poly *)malloc(sizeof(Poly));
    p->deg = deg;
    p->coeff = (seq_t *)calloc((size_t)(deg + 1), sizeof(seq_t));
    return p;
}

/* Free a polynomial and its coefficient array. */
static void poly_free(Poly *p)
{
    if (!p) return;
    free(p->coeff);
    free(p);
}

/* Polynomial multiplication (both factors are monic). */
static Poly *poly_mul(const Poly *a, const Poly *b)
{
    idx_t deg = a->deg + b->deg;
    Poly *p = poly_new(deg);
    for (idx_t i = 0; i <= a->deg; ++i) {
        for (idx_t j = 0; j <= b->deg; ++j) {
            p->coeff[i + j] += a->coeff[i] * b->coeff[j];
        }
    }
    return p;
}

/* Polynomial division: num / den, assuming exact division and monic den. */
static Poly *poly_div(const Poly *num, const Poly *den)
{
    idx_t degq = num->deg - den->deg;
    /* Temporary copy of numerator coefficients. */
    seq_t *tmp = (seq_t *)calloc((size_t)(num->deg + 1), sizeof(seq_t));
    for (idx_t i = 0; i <= num->deg; ++i) tmp[i] = num->coeff[i];

    seq_t *quot = (seq_t *)calloc((size_t)(degq + 1), sizeof(seq_t));

    for (idx_t i = num->deg; i >= den->deg; --i) {
        seq_t coeff = tmp[i];          /* leading coefficient at this point */
        if (coeff != 0) {
            quot[i - den->deg] = coeff;
            for (idx_t j = 0; j <= den->deg; ++j) {
                tmp[i - den->deg + j] -= coeff * den->coeff[j];
            }
        }
    }

    /* Build quotient polynomial from temporary array. */
    Poly *q = poly_new(degq);
    for (idx_t i = 0; i <= degq; ++i) q->coeff[i] = quot[i];

    free(tmp);
    free(quot);
    return q;
}

/* Construct polynomial x^p – 1. */
static Poly *poly_x_pow_minus_one(idx_t p)
{
    Poly *poly = poly_new(p);
    poly->coeff[0] = -1;           /* constant term */
    if (p > 0) poly->coeff[p] = 1; /* leading term */
    return poly;
}

/* --------------------------------------------------------------------- */
/* Storage, generation and caching of Φ_952 and its coefficients.        */
static bool cyclo_generated = false;
static idx_t  phi_deg   = 0;
static seq_t *phi_coeff = NULL;  /* points to array of size phi_deg+1 */

/* Utility: find index of value v in sorted array a[0..cnt-1] */
static int index_of(const idx_t *a, int cnt, idx_t v)
{
    for (int i = 0; i < cnt; ++i) {
        if (a[i] == v) return i;
    }
    return -1;   /* should never happen for correct divisor list */
}

/* Generate cyclotomic polynomial Φ_952 and its coefficients once. */
static void generate_cyclo_952(void)
{
    if (cyclo_generated) return;

    /* Enumerate all divisors of 952 = 2^3·7·17. */
    idx_t divisors[MAX_DIV];
    int div_cnt = 0;
    for (int i = 0; i <= 3; ++i) {           /* powers of 2: 1, 2, 4, 8 */
        for (int j = 0; j <= 1; ++j) {       /* 7^0 or 7^1 */
            for (int k = 0; k <= 1; ++k) {   /* 17^0 or 17^1 */
                idx_t val = ((idx_t)1 << i) * (j ? 7 : 1) * (k ? 17 : 1);
                divisors[div_cnt++] = val;
            }
        }
    }
    /* Simple insertion sort for ascending order (div_cnt <= MAX_DIV). */
    for (int i = 1; i < div_cnt; ++i) {
        idx_t key = divisors[i];
        int j = i - 1;
        while (j >= 0 && divisors[j] > key) {
            divisors[j + 1] = divisors[j];
            --j;
        }
        divisors[j + 1] = key;
    }

    /* Temporary array of polynomials for each divisor. */
    Poly *poly_arr[MAX_DIV] = {NULL};

    /* Compute cyclotomic polynomials in ascending order. */
    for (int idx = 0; idx < div_cnt; ++idx) {
        idx_t n = divisors[idx];

        if (n == 1) {    /* Φ_1(x) = x - 1 */
            Poly *p = poly_new(1);
            p->coeff[0] = -1;
            p->coeff[1] =  1;
            poly_arr[idx] = p;
            continue;
        }

        /* numerator: x^n – 1 */
        Poly *numer = poly_x_pow_minus_one(n);

        /* product of Φ_d for all proper divisors d|n, d < n */
        Poly *product = poly_new(0);             /* start with polynomial 1 */
        product->coeff[0] = 1;

        for (int j = 0; j < idx; ++j) {
            idx_t d = divisors[j];
            if (n % d == 0) {
                Poly *old = product;
                product = poly_mul(product, poly_arr[j]);
                poly_free(old);
            }
        }

        /* quotient: Φ_n = (x^n - 1) / product */
        Poly *phi_n = poly_div(numer, product);
        poly_arr[idx] = phi_n;

        poly_free(numer);
        poly_free(product);
    }

    /* Locate the index of 952 in the divisor list. */
    int pos_952 = index_of(divisors, div_cnt, 952);
    Poly *phi_952 = poly_arr[pos_952];

    /* Store coefficients permanently. */
    phi_deg   = phi_952->deg;
    phi_coeff = phi_952->coeff;  /* take ownership of the array */

    /* Free all other temporary polynomials. */
    for (int i = 0; i < div_cnt; ++i) {
        if (i == pos_952) {                /* keep coefficients only */
            free(poly_arr[i]);             /* free the struct, not array */
        } else {
            poly_free(poly_arr[i]);        /* free both struct and array */
        }
    }

    cyclo_generated = true;
}

/* --------------------------------------------------------------------- */
/* Public sequence function. */
seq_t a(idx_t n)
{
    generate_cyclo_952();       /* ensure Φ_952 coefficients ready */

    if (n == 0) return 1;

    /* Allocate array for a[0..n]. */
    seq_t *aseq = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    aseq[0] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t kmax = (i < phi_deg) ? i : phi_deg;
        for (idx_t k = 1; k <= kmax; ++k) {
            sum += phi_coeff[k] * aseq[i - k];
        }
        aseq[i] = -sum;
    }

    seq_t result = aseq[n];
    free(aseq);
    return result;
}
