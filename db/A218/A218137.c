/*
 * A218137  Sum of absolute values of real and imaginary parts of the
 *          coefficients in the expansion of 1 / (1 - x - I*x^2), where
 *          I^2=-1.
 * https://oeis.org/A218137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |Re(c_n)| + |Im(c_n)|, where c_0 = 1, c_1 = 1 and c_n =
 *            c_{n-1} + i * c_{n-2} (I^2 = -1)
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 60, u64 n <= 121,
 *            u128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Signed sequence element: magnitude and sign (-1,0,1).  */
typedef struct {
    seq_t mag;  /* magnitude (non‑negative) */
    int   sign; /* -1, 0, or +1 */
} signed_seq;

/* Pair of real and imaginary signed sequences. */
typedef struct {
    signed_seq a; /* real part */
    signed_seq b; /* imaginary part */
} coeff_pair;

/* ---------- helpers on signed_seq ---------- */

static inline void signed_zero(signed_seq *s) {
    s->mag  = 0;
    s->sign = 0;
}

static inline void signed_copy(const signed_seq *src, signed_seq *dst) {
    dst->mag  = src->mag;
    dst->sign = src->sign;
}

static inline void signed_neg(const signed_seq *s, signed_seq *res) {
    if (s->sign == 0) {
        res->mag  = 0;
        res->sign = 0;
    } else {
        res->mag  = s->mag;
        res->sign = -s->sign;
    }
}

/* Add two signed_seq numbers: res = x + y */
static inline void signed_add(const signed_seq *x, const signed_seq *y,
                              signed_seq *res) {
    int sx = x->sign, sy = y->sign;
    if (sx == 0) {
        signed_copy(y, res);
        return;
    }
    if (sy == 0) {
        signed_copy(x, res);
        return;
    }
    if (sx == sy) { /* same sign */
        res->mag  = x->mag + y->mag;
        res->sign = sx;
    } else { /* opposite signs – subtract magnitudes */
        if (x->mag > y->mag) {
            res->mag  = x->mag - y->mag;
            res->sign = sx;
        } else if (x->mag < y->mag) {
            res->mag  = y->mag - x->mag;
            res->sign = sy;
        } else { /* equal magnitudes -> zero */
            res->mag  = 0;
            res->sign = 0;
        }
    }
}

/* Subtract two signed_seq numbers: res = x - y */
static inline void signed_sub(const signed_seq *x, const signed_seq *y,
                              signed_seq *res) {
    signed_seq ny;
    signed_neg(y, &ny);
    signed_add(x, &ny, res);
}

/* ---------- cached coefficient table ---------- */

static coeff_pair *cache = NULL;     /* array of coefficients */
static idx_t      cache_len = 0;     /* number of elements allocated */

/* Ensure that terms up to n (inclusive) are computed and cached.  */
static void ensure_cache(idx_t n) {
    if (cache != NULL && cache_len > n)
        return;                      /* already enough */

    size_t new_len = (size_t)(n + 1);
    coeff_pair *new_cache =
        (coeff_pair *)realloc(cache, new_len * sizeof(coeff_pair));
    if (new_cache == NULL)             /* allocation failed – abort */
        return;
    cache = new_cache;

    /* Initialise newly allocated entries to zero. */
    for (size_t i = (size_t)cache_len; i < new_len; ++i) {
        signed_zero(&cache[i].a);
        signed_zero(&cache[i].b);
    }

    /* Base cases must always be correct. */
    /* c0 = 1 + 0i */
    cache[0].a.mag  = 1;
    cache[0].a.sign = 1;
    cache[0].b.mag  = 0;
    cache[0].b.sign = 0;
    /* c1 = 1 + 0i */
    cache[1].a.mag  = 1;
    cache[1].a.sign = 1;
    cache[1].b.mag  = 0;
    cache[1].b.sign = 0;

    /* Compute any terms that are not yet computed. */
    idx_t start = (cache_len > 2) ? cache_len : 2;
    for (idx_t k = start; k <= n; ++k) {
        /* a_k = a_{k-1} - b_{k-2} */
        signed_sub(&cache[k-1].a, &cache[k-2].b, &cache[k].a);
        /* b_k = b_{k-1} + a_{k-2} */
        signed_add(&cache[k-1].b, &cache[k-2].a, &cache[k].b);
    }

    cache_len = new_len;
}

/* ---------- public function ---------- */

seq_t a(idx_t n) {
    if (n == 0u)
        return 1;                              /* a(0) = 1 by definition */

    ensure_cache(n);

    /* |a_n| + |b_n| */
    seq_t ar = cache[n].a.mag;
    seq_t ai = cache[n].b.mag;
    return ar + ai;
}
