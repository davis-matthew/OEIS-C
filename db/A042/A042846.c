/*
 * A042846  Numerators of continued fraction convergents to sqrt(954).
 * https://oeis.org/A042846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 64160102*a(n-14) - a(n-28) (n > 27) initial terms
 *            a(0)…a(27) are numerators of convergents of sqrt(954)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for the generated sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* recurrence coefficient as a seq_t value */
static const seq_t coeff = 64160102U;

/* compute the first 28 terms via continued‑fraction convergents of sqrt(954) */
static void init_initial_terms(void)
{
    /* integer floor of sqrt(954) without floating point */
    unsigned int a0 = 0;
    while ((a0 + 1u) * (a0 + 1u) <= 954U) {
        ++a0;
    }

    /* partial quotients of the continued fraction of sqrt(954) */
    unsigned int partial[28];
    partial[0] = a0;

    unsigned int m = 0;
    unsigned int d = 1;
    unsigned int a = a0;
    for (int i = 1; i < 28; ++i) {
        m = d * a - m;
        d = (954U - m * m) / d;
        a = (a0 + m) / d;
        partial[i] = a;
    }

    /* convergent numerators */
    seq_t p_minus2 = 0;  /* p_{-2} */
    seq_t p_minus1 = 1;  /* p_{-1} */
    for (size_t i = 0; i < 28; ++i) {
        cache[i] = (seq_t)partial[i] * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = cache[i];
    }
    cache_len = 28;
}

seq_t a(idx_t n)
{
    /* allocate or grow the cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 32;
        if (new_cap < required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise with the first 28 terms if the cache is empty */
    if (cache_len == 0) {
        cache = (seq_t *)calloc(28, sizeof(seq_t));
        if (cache == NULL) abort();
        init_initial_terms();
    }

    /* compute further terms using the linear recurrence */
    if ((size_t)n >= cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t prod = coeff * cache[i - 14];
            cache[i] = prod - cache[i - 28];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
