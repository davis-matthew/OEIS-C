/*
 * A041573  Denominators of continued fraction convergents to sqrt(304).
 * https://oeis.org/A041573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041573: denominators of continued‑fraction convergents to sqrt(304)
   Recurrence: a(n) = 115598*a(n-12) – a(n-24)  for n > 23
   Initial values (n=0…23) are derived from the continued fraction of sqrt(304). */

#include <stdlib.h>           /* malloc, realloc, exit */

/* Static cache used by the sequence implementation. */
static seq_t *cache = NULL;    /* array of computed terms */
static idx_t cache_size = 0;   /* number of stored terms */
static idx_t cache_cap  = 0;   /* allocated capacity */

/* Ensure the cache has space for at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 24;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the first 24 denominators via the continued‑fraction algorithm for sqrt(304). */
static void init_cache(void)
{
    if (cache != NULL)            /* already initialised */
        return;

    const idx_t D    = 304;        /* the square‑free part */
    const idx_t a0   = 17;         /* floor(sqrt(304)) */
    idx_t m = 0, d = 1, a = a0;    /* initial continued‑fraction parameters */

    cache_cap   = 24;
    cache_size  = 24;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        exit(EXIT_FAILURE);

    /* Convergents: initialise with the first term [a0] */
    seq_t p_minus2 = 0, p_minus1 = 1;
    seq_t q_minus2 = 1, q_minus1 = 0;

    seq_t p = (seq_t)a * p_minus1 + p_minus2;  /* p0 = a0 * 1 + 0 */
    seq_t q = (seq_t)a * q_minus1 + q_minus2;  /* q0 = a0 * 0 + 1 = 1 */
    cache[0] = q;                                /* a(0) = 1 */

    /* Update registers for next iteration */
    p_minus2 = p_minus1;  p_minus1 = p;
    q_minus2 = q_minus1;  q_minus1 = q;

    /* Generate coefficients a1 … a23 and corresponding denominators */
    for (idx_t k = 1; k < 24; ++k) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        p = (seq_t)a * p_minus1 + p_minus2;
        q = (seq_t)a * q_minus1 + q_minus2;
        cache[k] = q;

        /* shift registers for next step */
        p_minus2 = p_minus1;  p_minus1 = p;
        q_minus2 = q_minus1;  q_minus1 = q;
    }
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    init_cache();                      /* ensure base values are ready */

    /* Make sure the cache can hold up to 'n'. */
    ensure_capacity(n + 1);

    /* Compute missing values up to 'n' using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i >= 24) {                 /* recurrence valid for n > 23 */
            seq_t term = (seq_t)115598 * cache[i - 12] - cache[i - 24];
            cache[i] = term;
        }
        /* For i < 24 no computation needed – initial values exist. */
    }
    if (n >= cache_size)
        cache_size = n + 1;

    return cache[n];
}
