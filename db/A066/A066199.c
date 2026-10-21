/*
 * A066199  a(1) = 1; for n > 1, a(n) = a(n-1)-(n+1) if this is positive and
 *          has not already appeared in the sequence, otherwise a(n) =
 *          a(n-1)+(n+1).
 * https://oeis.org/A066199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066199
   a(1) = 1
   for n > 1, let k = n + 1
   a(n) = a(n-1) - k if a(n-1) - k > 0 and the result has not already appeared in the sequence;
   otherwise a(n) = a(n-1) + k
*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ------------- cache for already computed terms ------------- */
    static seq_t *cache = NULL;   /* cache[0] holds a(1) */
    static size_t  cached = 0;   /* number of computed terms      */
    static size_t  cap    = 0;   /* capacity of cache             */

    if (n == 0)                     /* undefined for n < 1, avoid UB   */
        return 0;

    /* ---------- initialise cache on first call ------------------- */
    if (cached == 0) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;        /* out‑of‑memory: give up */
        cache[0] = (seq_t)1;         /* a(1) */
        cached = 1;
    }

    /* ------------- grow cache if necessary ---------------------- */
    while (cached < (size_t)n) {
        /* grow storage */
        if (cached >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap < cached + 1) new_cap = cached + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* out‑of‑memory: give up */
            cache = tmp;
            cap = new_cap;
        }

        /* index of the term we are about to compute (1‑based) */
        idx_t i = (idx_t)(cached + 1);   /* cached holds a(i-1) */

        /* the step value is i + 1 */
        seq_t step = (seq_t)(i + 1);

        seq_t prev = cache[cached - 1];
        bool subtract_ok = false;

        if (prev > step) {            /* candidate positive */
            seq_t cand = prev - step;

            /* check if cand has already appeared */
            bool seen = false;
            for (size_t j = 0; j < cached; ++j) {
                if (cache[j] == cand) { seen = true; break; }
            }
            if (!seen) subtract_ok = true;
        }

        cache[cached] = subtract_ok ? (prev - step) : (prev + step);
        ++cached;
    }

    return cache[n - 1];
}
