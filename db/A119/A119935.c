/*
 * A119935  Triangle of numerators of the cube of a certain lower triangular
 *          matrix.
 * https://oeis.org/A119935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A119935 - Triangle of numerators of the cube of a lower‑triangular matrix
 *  a(n) is the numerator of the rational entry r(i,j) of
 *          (A^3)[i,j]
 *  where A[i,j] = 1/i for j ≤ i, 0 otherwise.  The pair (i,j) is obtained
 *  from the index n by the usual triangle numbering:
 *
 *          i = smallest integer with i(i+1)/2 ≥ n
 *          j = n – i(i-1)/2   (1 ≤ j ≤ i)
 *
 *  The computation uses exact arithmetic on seq_t (any integer type that the
 *  build supplies).  It keeps a cache of the harmonic numbers H[i] =
 *  Σ_{t=1}^{i} 1/t and the auxiliary sums S1[i] = Σ_{t=1}^{i} H[t]/t to
 *  evaluate r(i,j) in O(1) after an O(i) precomputation step.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Fraction type for rational arithmetic */
typedef struct { seq_t num; seq_t den; } frac_t;

/* Reduce fraction to lowest terms */
static inline void frac_reduce(frac_t *f) {
    seq_t g = gcd(f->num, f->den);
    f->num /= g;
    f->den /= g;
}

/* Addition of two fractions: out = a + b */
static inline void frac_add(const frac_t *a, const frac_t *b, frac_t *out) {
    seq_t g = gcd(a->den, b->den);
    seq_t mul_a = a->num * (b->den / g);
    seq_t mul_b = b->num * (a->den / g);
    out->num = mul_a + mul_b;
    out->den = (a->den / g) * b->den;
    frac_reduce(out);
}

/* Subtraction of two fractions: out = a - b */
static inline void frac_sub(const frac_t *a, const frac_t *b, frac_t *out) {
    seq_t g = gcd(a->den, b->den);
    seq_t mul_a = a->num * (b->den / g);
    seq_t mul_b = b->num * (a->den / g);
    out->num = mul_a - mul_b;
    out->den = (a->den / g) * b->den;
    frac_reduce(out);
}

/* Multiplication of two fractions: out = a * b */
static inline void frac_mul(const frac_t *a, const frac_t *b, frac_t *out) {
    out->num = a->num * b->num;
    out->den = a->den * b->den;
    frac_reduce(out);
}

/* Division of a fraction by an integer: out = a / m  (m > 0) */
static inline void frac_div_int(const frac_t *a, idx_t m, frac_t *out) {
    out->num = a->num;
    out->den = a->den * (seq_t)m;
    frac_reduce(out);
}

/* Global cache for harmonic numbers H[i] and auxiliary sums S1[i] */
static frac_t *H = NULL;      /* H[i]  = Σ_{t=1}^{i} 1/t              */
static frac_t *S1 = NULL;     /* S1[i] = Σ_{t=1}^{i} H[t]/t            */
static size_t capacity = 0;   /* allocated size of H and S1 arrays     */
static idx_t  current_max = 0;/* largest i for which arrays are defined */

/* Ensure that arrays H and S1 are allocated up to at least 'need' elements */
static void ensure_capacity(idx_t need) {
    if (capacity > (size_t)need) return;

    size_t new_cap = capacity ? capacity * 2 : 8;
    while (new_cap <= (size_t)need) new_cap *= 2;

    frac_t *tmpH  = (frac_t*)realloc(H,  new_cap * sizeof(frac_t));
    frac_t *tmpS1 = (frac_t*)realloc(S1, new_cap * sizeof(frac_t));

    H   = tmpH;
    S1  = tmpS1;
    capacity = new_cap;

    /* initialise the zero element (already done on first allocation) */
    if (current_max == 0) {
        H[0].num = 0;  H[0].den = 1;
        S1[0].num = 0; S1[0].den = 1;
        current_max = 0;
    }
}

/* Pre‑compute harmonic numbers and S1 sums up to index 'max_i' (inclusive) */
static void precompute(idx_t max_i) {
    if (max_i <= current_max) return;
    ensure_capacity(max_i);

    for (idx_t k = current_max + 1; k <= max_i; ++k) {
        /* H[k] = H[k-1] + 1/k */
        frac_t one_over_k = { (seq_t)1, (seq_t)k };
        frac_add(&H[k-1], &one_over_k, &H[k]);

        /* temp = H[k] / k  ( = H[k] * (1/k) ) */
        frac_t temp;
        frac_div_int(&H[k], k, &temp);

        /* S1[k] = S1[k-1] + temp */
        frac_add(&S1[k-1], &temp, &S1[k]);
    }

    current_max = max_i;
}

/* Convert linear index n to matrix coordinates (i,j) (1‑based) */
static void index_to_ij(idx_t n, idx_t *i_out, idx_t *j_out) {
    idx_t i = 0;
    while ((i * (i + 1)) / 2 < n) ++i;
    *i_out = i;
    idx_t offset_prev = i * (i - 1) / 2;
    *j_out = n - offset_prev;
}

/* Main sequence routine */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* n < 1 is undefined; return 0 */

    idx_t i, j;
    index_to_ij(n, &i, &j);

    precompute(i);   /* ensure H[i] and S1[i] are ready */

    /* D = S1[i] - S1[j-1] */
    frac_t diffS1;
    frac_sub(&S1[i], &S1[j-1], &diffS1);

    /* E = H[i] - H[j-1] */
    frac_t diffH;
    frac_sub(&H[i], &H[j-1], &diffH);

    /* F = H[j-1] * E */
    frac_t prod;
    frac_mul(&H[j-1], &diffH, &prod);

    /* G = D - F  (this is r(i,j) * i) */
    frac_t gnum;
    frac_sub(&diffS1, &prod, &gnum);

    /* r(i,j) = gnum / i */
    seq_t new_den = gnum.den * (seq_t)i;
    seq_t g = gcd(gnum.num, new_den);

    return gnum.num / g;
}
