/*
 * A137677  Expansion of f(-x, -x^4) / psi(-x) where psi() is a Ramanujan
 *          theta function and f(, ) is Ramanujan's general theta function.
 * https://oeis.org/A137677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in the series expansion of *
 *            f(-x,-x^4)/psi(-x), where f(a,b) is Ramanujan's general theta
 *            function * and psi is Ramanujan's psi function. * * The sequence
 *            satisfies: * a(4*n+1) = a(4*n+2) = 0 * a(4*n) = A122134(n) *
 *            a(4*n+3) = A122130(n) * * The following implement...
 * Range:     u8 n <= 107, u16 n <= 278, u32 n <= 819, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static int8_t *num_sign = NULL;          /* numerator coefficient signs (+1, -1, 0) */
static int8_t *den_sign = NULL;          /* denominator coefficient signs (+1, -1, 0) */
static seq_t *cache    = NULL;           /* computed a(n) values           */
static idx_t  cache_len = 0;             /* length of cache array          */

/* Expand the generating function up to index N, filling cache[0..N] */
static void expand_to(idx_t N)
{
    size_t len = (size_t)N + 1;
    /* Resize sign arrays */
    if (num_sign == NULL) {
        num_sign = (int8_t*)calloc(len, sizeof(int8_t));
        den_sign = (int8_t*)calloc(len, sizeof(int8_t));
    } else if (cache_len < len) {
        num_sign = (int8_t*)realloc(num_sign, len * sizeof(int8_t));
        den_sign = (int8_t*)realloc(den_sign, len * sizeof(int8_t));
        for (size_t i = cache_len; i < len; ++i) {
            num_sign[i] = 0;
            den_sign[i] = 0;
        }
    }

    /* Reset sign arrays */
    for (size_t i = 0; i < len; ++i) {
        num_sign[i] = 0;
        den_sign[i] = 0;
    }

    /* Build signs for the numerator f(-x,-x^4) */
    for (uint64_t k = 0;; ++k) {
        uint64_t e_pos = (5*k*k - 3*k) / 2;
        if (e_pos > N) break;
        num_sign[e_pos] = (k & 1U) ? -1 : 1;
        if (k > 0) {
            uint64_t e_neg = (5*k*k + 3*k) / 2;
            if (e_neg <= N)
                num_sign[e_neg] = (k & 1U) ? -1 : 1;
        }
    }

    /* Build signs for the denominator psi(-x) */
    for (uint64_t k = 0;; ++k) {
        uint64_t t = k * (k + 1) / 2;
        if (t > N) break;
        int sgn = (t & 1ULL) ? -1 : 1;   /* sign = (-1)^{t} */
        den_sign[t] = (int8_t)sgn;
    }

    /* Resize cache array */
    seq_t *new_cache = (seq_t*)realloc(cache, len * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Out of memory; abort since we cannot continue */
        abort();
    }
    cache = new_cache;

    /* Compute coefficients a(0..N) */
    for (idx_t i = cache_len; i <= N; ++i) {
        seq_t s1 = 0;  /* sum of a[i-j] where den_sign[j] ==  1 */
        seq_t s2 = 0;  /* sum of a[i-j] where den_sign[j] == -1 */
        for (idx_t j = 1; j <= i; ++j) {
            if (den_sign[j] ==  1) s1 += cache[i - j];
            else if (den_sign[j] == -1) s2 += cache[i - j];
        }
        seq_t diff = s2 - s1;          /* s2 >= s1 is guaranteed by the
                                         * construction of the series */
        switch (num_sign[i]) {
            case  1: cache[i] = diff + 1; break;
            case -1: cache[i] = diff - 1; break;
            default : cache[i] = diff;     break;
        }
    }
    cache_len = len;
}

/* Requested function as required by the problem statement */
seq_t a(idx_t n)
{
    if (n >= cache_len)
        expand_to(n);
    return cache[n];
}
