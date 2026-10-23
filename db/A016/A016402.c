/*
 * A016402  406th cyclotomic polynomial.
 * https://oeis.org/A016402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 406th cyclotomic polynomial
 *            Φ_406(x)
 * Range:     i8 n <= 168, i16 n <= 168, i32 n <= 168, i64 n <= 168,
 *            i128 n <= 168, big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm builds Φ_406 polynomial on first call and returns its n‑th coefficient. */
/* It uses the division formula: Φ_n(x)·∏_{d|n,d<n} Φ_d(x) = x^n – 1. */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

/* Structure to cache a cyclotomic polynomial. */
typedef struct {
    idx_t n;            /* order of the cyclotomic polynomial */
    seq_t *coef;        /* coefficient array, coef[0] = constant term */
    size_t deg;         /* degree of the polynomial */
} Poly;

/* Maximum number of cached polynomials needed (here 10 is sufficient). */
#define MAX_CACHE 10
static Poly cache[MAX_CACHE];
static size_t cache_count = 0;

/* Forward declaration */
static Poly get_poly(idx_t n);

/* Multiply polynomials A (degA) and B (degB), store result in C (length degA+degB+1). */
static void poly_mul(const seq_t *A, size_t degA,
                     const seq_t *B, size_t degB,
                     seq_t *C)
{
    for (size_t i = 0; i <= degA; ++i) {
        for (size_t j = 0; j <= degB; ++j) {
            C[i + j] += A[i] * B[j];
        }
    }
}

/* Retrieve or compute Φ_n(x). Returns a pointer to a cached Poly. */
static Poly get_poly(idx_t n)
{
    /* Check cache */
    for (size_t i = 0; i < cache_count; ++i) {
        if (cache[i].n == n) return cache[i];
    }

    /* Base case: Φ_1(x) = x – 1. */
    if (n == 0) {
        /* by convention Φ_0 is undefined; not used here. */
        Poly res = {0, NULL, 0};
        return res;
    }
    if (n == 1) {
        seq_t *arr = alloc_seq_array(2);
        arr[0] = (seq_t)-1;
        arr[1] = (seq_t)1;
        Poly res = {1, arr, 1};
        if (cache_count < MAX_CACHE) cache[cache_count++] = res;
        return res;
    }

    /* Build product of Φ_d(x) over proper divisors d < n. */
    size_t degD = 0;
    seq_t *prod = alloc_seq_array(1);
    prod[0] = (seq_t)1;   /* start with polynomial 1 */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        if (d < n) {
            Poly pd = get_poly(d);
            seq_t *newprod = alloc_seq_array(degD + pd.deg + 1);
            poly_mul(prod, degD, pd.coef, pd.deg, newprod);
            free_seq(prod);
            prod = newprod;
            degD += pd.deg;
        }
        idx_t d2 = n / d;
        if (d2 != d && d2 < n) {
            Poly pd = get_poly(d2);
            seq_t *newprod = alloc_seq_array(degD + pd.deg + 1);
            poly_mul(prod, degD, pd.coef, pd.deg, newprod);
            free_seq(prod);
            prod = newprod;
            degD += pd.deg;
        }
    }

    /* Numerator: x^n – 1 */
    seq_t *num = alloc_seq_array((size_t)n + 1);
    for (size_t i = 0; i <= (size_t)n; ++i) num[i] = (seq_t)0;
    num[n] = (seq_t)1;
    num[0] = (seq_t)-1;

    /* Long division: Quotient = num / prod. */
    size_t degQ = n - degD;
    seq_t *quot = alloc_seq_array(degQ + 1);
    for (size_t i = 0; i <= degQ; ++i) quot[i] = (seq_t)0;

    /* Perform division. */
    for (int i = (int)n; i >= (int)degD; --i) {
        seq_t coeff = num[i];
        size_t qpos = (size_t)(i - (int)degD);
        quot[qpos] = coeff;
        for (size_t j = 0; j <= degD; ++j) {
            num[qpos + j] -= coeff * prod[j];
        }
    }

    free_seq(num);
    free_seq(prod);

    Poly res = {n, quot, degQ};
    if (cache_count < MAX_CACHE) cache[cache_count++] = res;
    return res;
}

/* Static cache for Φ_406 polynomial. */
static Poly phi406 = {0, NULL, 0};
static int phi_built = 0;

/* The required function a(n). */
seq_t a(idx_t n)
{
    if (!phi_built) {
        phi406 = get_poly(406);
        phi_built = 1;
    }
    if (n <= (idx_t)phi406.deg) {
        return phi406.coef[n];
    }
    return (seq_t)0;
}
