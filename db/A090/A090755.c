/*
 * A090755  Denominator of the expansion of e^(x + x^2 + x^3 + x^4).
 * https://oeis.org/A090755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   (n+1)·a_{n+1} = a_n + 2a_{n-1} + 3a_{n-2} + 4a_{n-3} (a_0 = 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A090755 – denominator of the expansion of e^(x + x² + x³ + x⁴) */

#include <stddef.h>
#include <stdlib.h>

/* Simple Euclidean algorithm – works for any unsigned seq_t type. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

typedef struct
{
    seq_t num;   /* numerator */
    seq_t den;   /* denominator (always > 0) */
} rational_t;

/* Reduce a rational to lowest terms. */
static void rational_reduce(rational_t *r)
{
    seq_t g = gcd_seq_t(r->num, r->den);
    if (g > (seq_t)1) {
        r->num /= g;
        r->den /= g;
    }
}

/* r = a + b */
static void rational_add(rational_t *r, const rational_t *a, const rational_t *b)
{
    seq_t g = gcd_seq_t(a->den, b->den);
    seq_t l1 = a->den / g;
    seq_t l2 = b->den / g;
    r->den = a->den * l2;                /* common denominator = a.den * l2 = b.den * l1 */
    r->num = a->num * l2 + b->num * l1;   /* numerator over common denominator */
    rational_reduce(r);
}

/* r = a * m  (small integer, here 2,3, or 4) */
static void rational_mul_int(rational_t *r, const rational_t *a, unsigned int m)
{
    r->num = a->num * (seq_t)m;
    r->den = a->den;
    rational_reduce(r);
}

/* r = a / m  (integer division) */
static void rational_div_int(rational_t *r, const rational_t *a, unsigned int m)
{
    r->num = a->num;
    r->den = a->den * (seq_t)m;
    rational_reduce(r);
}

/* --------------------------------------------------------------------- */
/*  Static table for cached coefficients up to a pre‑selected limit.    */
/* --------------------------------------------------------------------- */
#define MAX_PRECOMPUTE 10000U
static rational_t coeff[MAX_PRECOMPUTE + 1];
static idx_t      cached_max = 0;   /* last index that has been computed */

/* Compute coefficients up to n (inclusive) using the recurrence. */
static void ensure_up_to(idx_t n)
{
    /* Initialise the base term once. */
    if (cached_max == 0 && coeff[0].den == (seq_t)0) {
        coeff[0].num = (seq_t)1;
        coeff[0].den = (seq_t)1;
    }

    if (n <= cached_max) return;          /* nothing to do */

    for (idx_t i = cached_max + 1; i <= n; ++i) {
        rational_t sum;
        sum.num = (seq_t)0;
        sum.den = (seq_t)1;

        if (i >= 1) rational_add(&sum, &sum, &coeff[i - 1]);
        if (i >= 2) {
            rational_t tmp; rational_mul_int(&tmp, &coeff[i - 2], 2);
            rational_add(&sum, &sum, &tmp);
        }
        if (i >= 3) {
            rational_t tmp; rational_mul_int(&tmp, &coeff[i - 3], 3);
            rational_add(&sum, &sum, &tmp);
        }
        if (i >= 4) {
            rational_t tmp; rational_mul_int(&tmp, &coeff[i - 4], 4);
            rational_add(&sum, &sum, &tmp);
        }
        rational_div_int(&coeff[i], &sum, (unsigned int)i);
    }
    cached_max = n;
}

/* --------------------------------------------------------------------- */
/*  Dynamic array for larger values – expands geometrically.            */
/* --------------------------------------------------------------------- */
static rational_t *big_coeff   = NULL;
static idx_t      big_size      = 0;
static idx_t      big_cached_max = 0;

static void big_ensure_up_to(idx_t n)
{
    if (big_coeff == NULL) {
        big_size = (n * 2 > MAX_PRECOMPUTE ? n * 2 : MAX_PRECOMPUTE * 2);
        big_coeff = (rational_t*)malloc(sizeof(rational_t) * (size_t)big_size);
        if (big_coeff == NULL) return;                /* allocation failure         */
        big_coeff[0].num = (seq_t)1;
        big_coeff[0].den = (seq_t)1;
        big_cached_max = 0;
    }

    /* Grow the buffer if necessary. */
    while (big_size <= n) {
        big_size *= 2;
        void *tmp = realloc(big_coeff, sizeof(rational_t) * (size_t)big_size);
        if (tmp == NULL) return;                     /* allocation failure         */
        big_coeff = (rational_t*)tmp;
    }

    if (n <= big_cached_max) return;

    for (idx_t i = big_cached_max + 1; i <= n; ++i) {
        rational_t sum;
        sum.num = (seq_t)0;
        sum.den = (seq_t)1;

        if (i >= 1) rational_add(&sum, &sum, &big_coeff[i - 1]);
        if (i >= 2) {
            rational_t tmp; rational_mul_int(&tmp, &big_coeff[i - 2], 2);
            rational_add(&sum, &sum, &tmp);
        }
        if (i >= 3) {
            rational_t tmp; rational_mul_int(&tmp, &big_coeff[i - 3], 3);
            rational_add(&sum, &sum, &tmp);
        }
        if (i >= 4) {
            rational_t tmp; rational_mul_int(&tmp, &big_coeff[i - 4], 4);
            rational_add(&sum, &sum, &tmp);
        }
        rational_div_int(&big_coeff[i], &sum, (unsigned int)i);
    }
    big_cached_max = n;
}

/* --------------------------------------------------------------------- */
/*  Public interface requested by the evaluation framework.              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= (idx_t)MAX_PRECOMPUTE) {
        ensure_up_to(n);
        return coeff[n].den;
    } else {
        big_ensure_up_to(n);
        return big_coeff[n].den;
    }
}
