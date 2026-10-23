/*
 * A041381  Denominators of continued fraction convergents to sqrt(205).
 * https://oeis.org/A041381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 79378*a(n-8) - a(n-16) (n > 15) Initial values are the
 *            first 16 denominators of the convergents to sqrt(205)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the first 16 denominators of the convergents to sqrt(205). */
static void compute_initial_terms(seq_t *out)
{
    const uint64_t D  = 205;
    const uint64_t a0 = 14;      /* floor(sqrt(205)) */

    uint64_t m = 0, d = 1, a = a0;

    /* P[-2]=0, P[-1]=1 ; Q[-2]=1, Q[-1]=0   */
    seq_t p_im2 = 0, p_im1 = 1;
    seq_t q_im2 = 1, q_im1 = 0;

    for (idx_t i = 0; i < 16; ++i) {
        seq_t p = (seq_t)a * p_im1 + p_im2;
        seq_t q = (seq_t)a * q_im1 + q_im2;
        out[i] = q;               /* denominator */

        /* shift for next step */
        p_im2 = p_im1;  p_im1 = p;
        q_im2 = q_im1;  q_im1 = q;

        /* update CF terms */
        if (i + 1 == 16) break;   /* last term computed */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }
}

/* Memoised computation of a(n). */
seq_t a(idx_t n)
{
    /* Static cache for computed values > 15. */
    static seq_t *cache       = NULL;   /* array [0..last] */
    static idx_t cache_len   = 0;   /* number of entries stored */
    static seq_t init[16];          /* first 16 terms */
    static int init_done = 0;

    /* Handle small indices directly. */
    if (n < 16) {
        if (!init_done) {
            compute_initial_terms(init);
            init_done = 1;
        }
        return init[n];
    }

    /* Ensure the cache is large enough for indices up to n. */
    if (cache_len <= n) {
        size_t new_len = (size_t)n + 1; /* allocate for indices 0..n */

        if (cache_len == 0) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!cache) abort();
            /* initialise first 16 terms */
            compute_initial_terms(init);
            for (idx_t i = 0; i < 16; ++i)
                cache[i] = init[i];
            cache_len = 16;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }

        /* Compute new terms using the 17‑term recurrence. */
        for (idx_t i = cache_len; i < new_len; ++i) {
            cache[i] = (seq_t)79378 * cache[i - 8] - cache[i - 16];
        }
        cache_len = new_len;
    }

    return cache[n];
}
