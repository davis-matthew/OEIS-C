/*
 * A014191  Inverse of 182nd cyclotomic polynomial.
 * https://oeis.org/A014191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 97 terms through n = 921 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014191 :
 *  Coefficients of the power‐series expansion of 1/Φ_182(x).
 *  The sequence is periodic with period 182.
 */
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Structure to cache a computed cyclotomic polynomial */
typedef struct {
    int n;          /* index of the polynomial Φ_n */
    seq_t *coeffs;  /* array of coefficients, coeffs[0] is constant term */
    size_t deg;     /* degree of Φ_n (coeffs[deg] == 1) */
} poly_t;

/* Maximum number of distinct Φ_n we may compute for n | 182 */
#define MAX_POLY 8
static poly_t poly_cache[MAX_POLY];
static size_t poly_cache_size = 0;

/* Forward declarations */
static seq_t *compute_phi(int n, size_t *deg_out);

/* Find cached polynomial Φ_n */
static poly_t *find_poly(int n)
{
    for (size_t i = 0; i < poly_cache_size; ++i)
        if (poly_cache[i].n == n)
            return &poly_cache[i];
    return NULL;
}

/* Add polynomial Φ_n to the cache (assuming it does not exist) */
static void add_poly(poly_t *p)
{
    if (poly_cache_size < MAX_POLY) {
        poly_cache[poly_cache_size++] = *p;
    }
    /* On overflow we simply leak the array – this path is never hit */
}

/* Compute Φ_n for a divisor of 182.  Results are cached.  */
static seq_t *compute_phi(int n, size_t *deg_out)
{
    poly_t *cached = find_poly(n);
    if (cached) {
        *deg_out = cached->deg;
        return cached->coeffs;
    }

    /* Base case: Φ_1(x) = x – 1 */
    if (n == 1) {
        seq_t *c = mem_new(seq_t, 2);
        c[0] = (seq_t)-1;
        c[1] = (seq_t)1;
        *deg_out = 1;
        poly_t p = {1, c, 1};
        add_poly(&p);
        return c;
    }

    /* Step 1: start with A(x) = x^n – 1 */
    seq_t *A = mem_new(seq_t, n + 1);
    A[0] = (seq_t)-1;
    A[n] = (seq_t)1;
    size_t degA = n;

    /* Step 2: divide by Φ_d for every proper divisor d of n (ascending) */
    for (int d = 1; d < n; ++d) {
        if (n % d != 0) continue;
        size_t degB;
        seq_t *B = compute_phi(d, &degB);  /* B is monic */

        /* Polynomial long division: A <- A / B (exact division) */
        size_t qdeg = degA - degB;          /* degree of the quotient   */
        seq_t *tmp = mem_new(seq_t, (degA + 1));
        mem_copy(tmp, A, (degA + 1) * sizeof(seq_t));

        seq_t *Q = mem_new(seq_t, qdeg + 1);

        /* Reverse loop over coefficients (highest degree first) */
        for (int i = (int)degA; i >= (int)degB; --i) {
            seq_t coeff = tmp[i];
            Q[i - (int)degB] = coeff;
            if (coeff != (seq_t)0) {
                for (size_t j = 0; j <= degB; ++j) {
                    size_t idx = (size_t)(i - (int)degB + j);
                    tmp[idx] -= coeff * B[j];
                }
            }
        }

        mem_free(tmp);
        mem_free(A);
        A = Q;
        degA = qdeg;
    }

    /* Now A contains Φ_n(x) */
    *deg_out = degA;
    poly_t p = {n, A, degA};
    add_poly(&p);
    return A;
}

/* Cache for the inverse coefficients (period 182) */
static seq_t *rev_cache = NULL;
static int rev_cache_inited = 0;

/* One‑time initializer: compute Φ_182 and its inverse power‑series */
static void init_rev_cache(void)
{
    /* Compute Φ_182 */
    size_t deg182;
    seq_t *phi = compute_phi(182, &deg182);   /* phi[0..deg182], phi[deg182] == 1 */

    /* Allocate array for inverse coefficients (period 182) */
    rev_cache = mem_new(seq_t, 182);
    rev_cache[0] = (seq_t)1;                  /* r_0 = 1 */

    /* Recurrence: r_m = -sum_{k=1}^{min(m,deg182)} phi[k] * r_{m-k} */
    for (int m = 1; m < 182; ++m) {
        seq_t sum = (seq_t)0;
        int limit = (m < (int)deg182) ? m : (int)deg182;
        for (int k = 1; k <= limit; ++k) {
            sum += phi[k] * rev_cache[m - k];
        }
        rev_cache[m] = -sum;                  /* r_m = -sum */
    }

    /* Free temporary polynomial data (except the needed rev_cache) */
    for (size_t i = 0; i < poly_cache_size; ++i)
        mem_free(poly_cache[i].coeffs);

    rev_cache_inited = 1;
}

/* Public sequence entry */
seq_t a(idx_t n)
{
    if (!rev_cache_inited)
        init_rev_cache();
    return rev_cache[n % 182UL];
}
