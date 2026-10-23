/*
 * A041858  Numerators of continued fraction convergents to sqrt(451).
 * https://oeis.org/A041858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the convergents of the continued fraction of
 *            sqrt(451). Computed by generating the partial quotients of
 *            sqrt(451)'s continued fraction and applying the recurrence p_k =
 *            a_k * p_{k-1} + p_{k-2}
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t *cache = NULL;
    static idx_t *partials = NULL;
    static idx_t cache_size = 0;
    static idx_t partials_size = 0;
    static idx_t max_index = 0;
    static idx_t m = 0;
    static idx_t d = 1;
    static const idx_t a0 = 21;   /* floor(sqrt(451)) */
    static idx_t a_curr = a0;
    static seq_t prev2 = 0;   /* p_{-2} */
    static seq_t prev1 = 1;   /* p_{-1} */

    /* First call: initialise cache for a(0) */
    if (!initialized)
    {
        cache_size = 1;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        partials_size = 1;
        partials = (idx_t*)malloc(partials_size * sizeof(idx_t));

        cache[0] = (seq_t)a0;      /* a(0) = 21 */
        partials[0] = a0;          /* first partial quotient a_0 */

        prev2 = 1;                 /* p_{-1} */
        prev1 = cache[0];          /* p_0 */

        max_index = 0;
        initialized = 1;
    }

    if (n <= max_index)
        return cache[n];

    /* Extend cache and partials up to n */
    for (idx_t i = max_index + 1; i <= n; ++i)
    {
        /* Generate next partial quotient for sqrt(451) */
        idx_t m_next = d * a_curr - m;
        idx_t d_next = ( (idx_t)451 - m_next * m_next ) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Resize partials array if necessary */
        if (i >= partials_size)
        {
            size_t new_size = partials_size ? partials_size * 2 : 1;
            idx_t *tmp = (idx_t*)realloc(partials, new_size * sizeof(idx_t));
            if (!tmp) return 0;  /* allocation failed */
            partials = tmp;
            partials_size = new_size;
        }
        partials[i] = a_next;

        /* Resize cache array if necessary */
        if (i >= cache_size)
        {
            size_t new_size = cache_size ? cache_size * 2 : 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
            cache_size = new_size;
        }

        /* Compute the numerator p_i */
        seq_t pn = a_next * prev1 + prev2;
        cache[i] = pn;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = pn;

        /* Update continued‑fraction state */
        m = m_next;
        d = d_next;
        a_curr = a_next;
    }

    max_index = n;
    return cache[n];
}
