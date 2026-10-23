/*
 * A042708  Numerators of continued fraction convergents to sqrt(884).
 * https://oeis.org/A042708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 3330*a(n-8) - a(n-16) for n ≥ 16. Initial values
 *            a(0)…a(15) are the numerators of the convergents to sqrt(884),
 *            generated from its continued‑fraction expansion
 * Range:     u8 none, u16 n <= 7, u32 n <= 20, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Generate the first 16 terms by the continued‑fraction recurrence. */
static void init_initial_terms(void)
{
    if (cache != NULL)
        return;                     /* already initialized */

    const seq_t D = (seq_t)884;        /* the radicand */
    cache_size = 16;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (cache == NULL)
        return;                      /* allocation failure – caller will return 0 */

    /* Continued‑fraction data for sqrt(D) */
    seq_t m  = 0;
    seq_t d  = 1;
    seq_t a0 = floor_sqrt(D);           /* floor(sqrt(884)) = 29 */
    seq_t a  = a0;

    /* Numerator recurrence: p_k = a_k*p_{k-1} + p_{k-2} */
    seq_t p_im2 = 0;   /* p_{-2} */
    seq_t p_im1 = 1;   /* p_{-1} */

    for (int i = 0; i < 16; ++i)
    {
        seq_t p = a * p_im1 + p_im2;   /* p_k */
        cache[i] = p;

        /* Shift p's for next iteration */
        seq_t p_next_im2 = p_im1;
        seq_t p_next_im1 = p;
        p_im2 = p_next_im2;
        p_im1 = p_next_im1;

        /* Compute next continued‑fraction coefficients */
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }
}

seq_t a(idx_t n)
{
    init_initial_terms();
    if (cache == NULL)
        return 0;                      /* memory allocation failed */

    /* If request is within already cached range. */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Need to extend the cache up to n. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        return cache[cache_size - 1];   /* cannot grow – return last cached value */
    cache = tmp;

    /* Compute missing terms using the recurrence a(k) = 3330*a(k-8) - a(k-16). */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        cache[i] = 3330 * cache[i - 8] - cache[i - 16];
    }
    cache_size = new_size;
    return cache[n];
}
