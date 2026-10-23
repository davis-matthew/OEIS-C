/*
 * A016155  Inverse of 2146th cyclotomic polynomial.
 * https://oeis.org/A016155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A016155: coefficients of the inverse of the 2146‑th cyclotomic
   polynomial Φ2146(x).  The sequence satisfies a(0)=1 and for n≥1

        a(n) = -∑_{k=1}^{deg Φ2146} φ_k · a(n-k)

   where φ_k are the coefficients of Φ2146(x) (φ_0 = 1).  The sequence is
   periodic with period 2146.
   The implementation computes Φ2146(x) by the recurrence

        Φ_N(x) = (x^N – 1) / ∏_{d|N, d<N} Φ_d(x)

   for N = 2146 and then builds the first 2146 terms of a(n) once.
*/

#include <stddef.h>
#include <stdbool.h>

/* Helper allocators supplied by the build environment */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Forward declaration of Polynomial type */
typedef struct {
    size_t deg;      /* Degree of the polynomial */
    seq_t *coeff;    /* coeff[i] is coefficient of x^i, 0 ≤ i ≤ deg */
} Poly;

/* Forward declarations of polynomial helpers */
static Poly* poly_mul(const Poly *a, const Poly *b);
static Poly* poly_div(const Poly *num, const Poly *den);
static Poly* poly_copy(const Poly *p);
static void   poly_free(Poly *p);

/* Cache for cyclotomic polynomials up to 2146 */
static Poly* cyclo_cache[2147] = {0};

/* Compute Φ_n(x) for 1 ≤ n ≤ 2146 */
static Poly* get_cyclotomic(idx_t n)
{
    if (cyclo_cache[n] != 0) return cyclo_cache[n];

    /* Special case Φ_1(x) = x – 1 */
    if (n == 1) {
        Poly *p = (Poly*)malloc(sizeof(Poly));
        p->deg    = 1;
        p->coeff  = alloc_seq_array(2);          /* size 2 */
        p->coeff[0] = -1;
        p->coeff[1] =  1;
        cyclo_cache[n] = p;
        return p;
    }

    /* Compute product of Φ_d(x) for all proper divisors d of n */
    Poly *prod = (Poly*)malloc(sizeof(Poly));
    prod->deg   = 0;
    prod->coeff = alloc_seq_array(1);            /* polynomial 1 */
    prod->coeff[0] = 1;

    for (idx_t d = 1; d < n; ++d) {
        if (n % d == 0) {
            Poly *child = get_cyclotomic(d);
            Poly *tmp   = poly_mul(prod, child);
            poly_free(prod);
            prod = tmp;
        }
    }

    /* numerator polynomial x^n – 1 */
    Poly *num = (Poly*)malloc(sizeof(Poly));
    num->deg   = n;
    num->coeff = alloc_seq_array(n + 1);
    for (size_t i = 0; i <= (size_t)n; ++i) num->coeff[i] = 0;
    num->coeff[0] = -1;
    num->coeff[n] =  1;

    /* Φ_n(x) = (x^n – 1) / prod */
    Poly *phi = poly_div(num, prod);

    poly_free(num);
    poly_free(prod);

    cyclo_cache[n] = phi;          /* store for future use */
    return phi;
}

/*----- Polynomial arithmetic ------------------------------------------------*/

static Poly* poly_copy(const Poly *p)
{
    Poly *r = (Poly*)malloc(sizeof(Poly));
    r->deg   = p->deg;
    r->coeff = alloc_seq_array(p->deg + 1);
    for (size_t i = 0; i <= p->deg; ++i) r->coeff[i] = p->coeff[i];
    return r;
}

static void poly_free(Poly *p)
{
    if (!p) return;
    free_seq_array(p->coeff);
    free(p);
}

static Poly* poly_mul(const Poly *a, const Poly *b)
{
    Poly *r = (Poly*)malloc(sizeof(Poly));
    r->deg   = a->deg + b->deg;
    r->coeff = alloc_seq_array(r->deg + 1);
    for (size_t i = 0; i <= r->deg; ++i) r->coeff[i] = 0;

    for (size_t i = 0; i <= a->deg; ++i) {
        for (size_t j = 0; j <= b->deg; ++j) {
            r->coeff[i + j] += a->coeff[i] * b->coeff[j];
        }
    }
    return r;
}

/*  Exact division of polynomials (num / den).  Assumes division is exact. */
static Poly* poly_div(const Poly *num, const Poly *den)
{
    size_t ndeg = num->deg, ddeg = den->deg;
    if (ddeg > ndeg) {                     /* return 0 polynomial */
        Poly *q = (Poly*)malloc(sizeof(Poly));
        q->deg   = 0;
        q->coeff = alloc_seq_array(1);
        q->coeff[0] = 0;
        return q;
    }

    size_t qdeg = ndeg - ddeg;
    Poly *q = (Poly*)malloc(sizeof(Poly));
    q->deg   = qdeg;
    q->coeff = alloc_seq_array(qdeg + 1);
    for (size_t i = 0; i <= qdeg; ++i) q->coeff[i] = 0;

    /* Working array of coefficients of the dividend */
    seq_t *tmp = alloc_seq_array(ndeg + 1);
    for (size_t i = 0; i <= ndeg; ++i) tmp[i] = num->coeff[i];

    for (int i = (int)ndeg; i >= (int)ddeg; --i) {
        seq_t c = tmp[i];
        if (c == 0) continue;
        q->coeff[i - ddeg] = c;
        for (size_t j = 0; j <= ddeg; ++j) {
            tmp[i - ddeg + j] -= c * den->coeff[j];
        }
    }
    free_seq_array(tmp);
    return q;
}

/*----- Sequence generator ---------------------------------------------------*/

seq_t a(idx_t n)
{
    static const idx_t PERIOD = 2146;
    static bool initialized = false;
    static seq_t *seq = NULL;          /* seq[0..PERIOD-1] */

    if (!initialized) {
        /* Compute Φ_2146(x) and its coefficients */
        Poly *phi = get_cyclotomic(PERIOD);
        size_t phi_deg = phi->deg;
        seq = alloc_seq_array(PERIOD);

        seq[0] = 1;                     /* a(0) = 1 */

        for (idx_t k = 1; k < PERIOD; ++k) {
            seq_t s = 0;
            size_t m = (k < phi_deg) ? (size_t)k : phi_deg;
            for (size_t i = 1; i <= m; ++i) {
                s += phi->coeff[i] * seq[k - i];
            }
            seq[k] = -s;                 /* a(k) = -∑ φ_i·a(k-i) */
        }

        /* The cyclotomic polynomial is no longer needed; keep it cached. */
        initialized = true;
    }

    return seq[n % PERIOD];
}
