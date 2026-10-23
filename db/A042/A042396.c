/*
 * A042396  Numerators of continued fraction convergents to sqrt(725).
 * https://oeis.org/A042396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 19602*a(n-6) – a(n-12) with initial values a(0) = 26,
 *            a(1) = 27, a(2) = 350, a(3) = 727, a(4) = 9074, a(5) = 9801,
 *            a(6) = 518726, a(7) = 528527, a(8) = 6861050, a(9) = 14250627,
 *            a(10) = 177868574, a(11) = 192119201 These initial values are
 *            generated algorithmically from the continued fract...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* ---------- compute the first 12 numerators from the continued
       fraction of sqrt(725) ----------
    */
    static seq_t first12[12];
    static int init_done = 0;

    if (!init_done) {
        const seq_t D   = (seq_t)725;   /* D = 25 * 29                         */
        const seq_t a0  = (seq_t)26;    /* floor(sqrt(725))                    */
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a_val = a0;               /* current partial quotient            */
        seq_t p_prevprev = (seq_t)0;    /* p_{k-2}                              */
        seq_t p_prev     = (seq_t)1;    /* p_{k-1}                              */

        for (idx_t i = 0; i < 12; ++i) {
            /* convergent numerator p_k = a_k * p_{k-1} + p_{k-2} */
            seq_t p = a_val * p_prev + p_prevprev;
            first12[i] = p;

            /* prepare next partial quotient only if needed */
            if (i < 11) {
                m = d * a_val - m;
                seq_t new_d = (D - m * m) / d;
                d = new_d;
                a_val = (a0 + m) / d;
                p_prevprev = p_prev;
                p_prev = p;
            }
        }
        init_done = 1;
    }

    /* ---------- memoized cache of computed terms ----------
       cache[0..cache_size-1] holds already computed a(k)   */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialize cache with the first 12 values */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return first12[0]; /* unlikely, safety guard */
        for (idx_t i = 0; i < 12; ++i) cache[i] = first12[i];
    }

    /* exact value already cached? */
    if (n < cache_size) return cache[n];

    /* extend cache up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure: allocate fresh & copy */
        new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return first12[0]; /* safety guard */
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 12) {
            new_cache[i] = first12[i];
        } else {
            new_cache[i] = ((seq_t)19602) * new_cache[i - 6] - new_cache[i - 12];
        }
    }

    /* finalise cache */
    cache = new_cache;
    cache_size = (idx_t)new_size;
    return cache[n];
}
