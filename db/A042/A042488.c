/*
 * A042488  Numerators of continued fraction convergents to sqrt(772).
 * https://oeis.org/A042488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042488: numerators of convergents of sqrt(772)
* Recurrence: a(n) = K*a(n-30) – a(n-60) for n > 59,
* where K = 12448646853698. Initial values a(0)…a(59)
* are the convergents of the continued fraction of sqrt(772). */
#include <stdlib.h>
#include <stdint.h>

/* compute the first 60 numerators from the continued‑fraction of sqrt(772) */
static void
init_base(seq_t *buf, idx_t upto)
{
    const idx_t D = 772;
    const idx_t a0 = 27;                     /* floor(sqrt(772)) */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    seq_t p_minus2 = 0;
    seq_t p_minus1 = 1;

    for (idx_t i = 0; i <= upto; ++i) {
        seq_t p = ((seq_t)a) * p_minus1 + p_minus2;
        buf[i] = p;

        /* update for next coefficient */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* shift the p sequence */
        p_minus2 = p_minus1;
        p_minus1 = p;
    }
}

/* cached values */
static seq_t *cache     = NULL;
static idx_t  cache_len = 0;

seq_t a(idx_t n)
{
    const seq_t K = (seq_t)12448646853698ULL;

    /* first time: allocate and compute the first 60 terms */
    if (cache == NULL) {
        cache_len = 60;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0;           /* allocation failed */
        init_base(cache, 59);           /* compute a[0] … a[59] */
    }

    /* grow cache if needed */
    if (n >= cache_len) {
        idx_t new_len = cache_len ? cache_len * 2 : 60;
        while (new_len <= n) new_len *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return 0;            /* allocation failed */
        cache = tmp;

        /* compute new terms using the recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            cache[i] = K * cache[i - 30] - cache[i - 60];
        }

        cache_len = new_len;
    }

    return cache[n];
}
