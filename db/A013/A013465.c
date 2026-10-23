/*
 * A013465  Expansion of e.g.f. cos(arctan(x) - arctanh(x)) (even powers
 *          only).
 * https://oeis.org/A013465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=0, a(2)=0, a(3)=-160. The sequence satisfies the
 *            recurrence a(n+4) = 8 * (2n+3)*(n+3)*(n+2)*(2n+7) * [
 *            -2*n*(n+1)*(1+2n)^2 * a(n) - 2*a(n+1) + a(n+2) ] for n ≥ 0.
 *            (Derived by cancelling common factors in the given D‑finite
 *            recurrence.)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values */
static size_t cache_size = 0;        /* number of terms stored */
static size_t cache_capacity = 0;    /* allocated capacity */

/* Ensure cache has at least 'needed' allocated slots. */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort as sequence cannot be computed. */
        abort();
    }
    cache = tmp;
    cache_capacity = new_cap;
}

/* Make sure cache contains values up to index 'n'. */
static void ensure_upto(idx_t n) {
    if (cache_size == 0) {
        /* Initial allocation and base values. */
        ensure_capacity(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)-160;
        cache_size = 4;
    }
    while (cache_size <= n) {
        idx_t i = cache_size - 4;              /* current n for recurrence */
        seq_t sn = (seq_t)i;

        /* Compute factors:
           coeffA = n*(n+1)*(1+2n)^2
           B      = (2n+3)*(n+3)*(n+2)*(2n+7)          */
        seq_t two_n_plus_1 = 2 * sn + 1;
        seq_t coeffA = sn * (sn + 1);
        coeffA = coeffA * two_n_plus_1 * two_n_plus_1;

        seq_t B = (2 * sn + 3) * (sn + 3) * (sn + 2) * (2 * sn + 7);

        /* Subexpression: -2*coeffA*a[i] - 2*a[i+1] + a[i+2] */
        seq_t sub = -2 * coeffA * cache[i];
        sub = sub - 2 * cache[i + 1] + cache[i + 2];

        /* Next term: a[i+4] = 8 * B * sub */
        seq_t next = 8 * B * sub;

        /* Store and grow cache if necessary */
        ensure_capacity(cache_size + 1);
        cache[cache_size] = next;
        cache_size++;
    }
}

seq_t a(idx_t n) {
    ensure_upto(n);
    return cache[n];
}
