/*
 * A041553  Denominators of continued fraction convergents to sqrt(294).
 * https://oeis.org/A041553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 9602*a(n-6) - a(n-12) for n > 11 with initial values:
 *            a(0)=1, a(1)=6, a(2)=7, a(3)=34, a(4)=41, a(5)=280, a(6)=9561,
 *            a(7)=57646, a(8)=67207, a(9)=326474, a(10)=393681, a(11)=2688560
 *            The function memoizes results so non‑monotonic calls are O(1)
 *            after the first computation up to the greatest...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache       = NULL;
    static size_t cache_cap   = 0;    /* current allocated size */
    static size_t cache_up_to = 0;    /* we have computed values [0,cache_up_to-1] */

    /* initial values (12 terms) */
    static const seq_t init[12] = {
        (seq_t)1,    (seq_t)6,    (seq_t)7,     (seq_t)34,
        (seq_t)41,   (seq_t)280,  (seq_t)9561,  (seq_t)57646,
        (seq_t)67207, (seq_t)326474, (seq_t)393681, (seq_t)2688560
    };

    /* first time: allocate cache and copy initial values */
    if (cache == NULL) {
        cache_cap   = 12;
        cache       = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        cache_up_to = 12; /* values [0,11] are ready */
    }

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* memory failure – unlikely in tests */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using recurrence */
    if (n >= cache_up_to) {
        const seq_t mult = (seq_t)9602;
        for (size_t i = cache_up_to; i <= (size_t)n; ++i) {
            cache[i] = mult * cache[i - 6] - cache[i - 12];
        }
        cache_up_to = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
