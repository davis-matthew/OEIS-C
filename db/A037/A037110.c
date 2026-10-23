/*
 * A037110  Trajectory of 3 under map n->25n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 34, u64 n <= 77, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 3
 *   a(n+1) = 25*a(n) + 1  if a(n) is odd
 *           = a(n)   / 2  if a(n) is even
 *   The function memoizes terms so any call order up to 10 000
 *   completes quickly.  Works for the 8–128‑bit unsigned integer
 *   variants and for the arbitrary‑precision build. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 0 ≤ i < cache_cap */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* number of elements currently stored */

    /* initialise cache for n = 0 */
    if (!cache) {
        cache = (seq_t*)calloc(1, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)3;
        cache_cap = 1;
    }

    /* grow cache if needed, remembering old capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;

        /* compute missing terms bootstrapped from old_cap */
        for (size_t i = old_cap; i < cache_cap; ++i) {
            seq_t prev = cache[i - 1];
            seq_t next;
            if (prev & 1u) {                /* odd */
                next = ((seq_t)25) * prev + 1;
            } else {                       /* even */
                next = prev >> 1;          /* same as prev / 2 */
            }
            cache[i] = next;
        }
    }

    return cache[(size_t)n];
}
