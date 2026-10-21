/*
 * A225570  The greedy smallest infinite reverse Collatz (3x+1) sequence.
 * https://oeis.org/A225570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 66, u32 n <= 116, u64 n <= 269,
 *            u128 n <= 542, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (a(n-1)-1)/3 if (a(n-1)-1)/3 is odd and not divisible by 3;
   otherwise a(n) = 2*a(n-1).
   Initial terms: a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=16. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* array indices start at 1; cache[0] is unused */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t last = 0;          /* largest n that has been computed */

    /* quick return for the first five terms */
    if (n <= 5) {
        /* values known a priori; they fit in any seq_t */
        static const seq_t init[6] = {0, 1, 2, 4, 8, 16};
        return init[(size_t)n];
    }

    /* if we have not allocated any storage yet, do it now */
    if (cache == NULL) {
        /* allocate space for indices 0..5 */
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();                 /* allocation failed */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 4;
        cache[4] = 8;
        cache[5] = 16;
        last = 5;
    }

    /* grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute all terms up to n if not yet done */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];

        if (prev % 3 == 1) {                 /* (prev-1) divisible by 3 */
            seq_t cand = (prev - 1) / 3;
            if ((cand % 2) == 1 && (cand % 3) != 0) {
                cache[i] = cand;             /* use the odd non‑multiple of 3 */
                continue;
            }
        }
        cache[i] = prev * 2;                 /* all other cases */
    }

    last = (size_t)n;
    return cache[(size_t)n];
}
