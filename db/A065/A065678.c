/*
 * A065678  Minimum value t such that all quadruples of Diffy_length >= n have
 *          a maximal value >= t.
 * https://oeis.org/A065678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3)+a(n-6)+a(n-9) for n >= 12 Initial values (OEIS
 *            A065678) for n = 0 .. 11: 0, 1, 1, 1, 1, 3, 3, 4, 9, 11, 13, 31
 *            The recurrence holds for all n >= 12
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 58, u64 n <= 112,
 *            u128 n <= 220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* array of computed terms   */
    static idx_t cache_size = 0;    /* number of terms stored    */

    /* pre‑initialized values for n = 0 .. 11 */
    static const seq_t base[] = {
        (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)3, (seq_t)3, (seq_t)4, (seq_t)9, (seq_t)11,
        (seq_t)13, (seq_t)31
    };
    const idx_t base_len = sizeof(base) / sizeof(base[0]);

    /* Return directly for the first twelve indices */
    if (n < base_len)
        return base[n];

    /* Allocate or grow cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise missing base values if the cache was empty or too small */
        if (cache_size < base_len) {
            idx_t i;
            for (i = cache_size; i < base_len; ++i)
                cache[i] = base[i];
        }

        /* Compute new terms using the recurrence */
        idx_t start = (cache_size > base_len) ? cache_size : base_len;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = cache[i-3] * (seq_t)3 + cache[i-6] + cache[i-9];
        }

        cache_size = new_size;
    }

    return cache[n];
}
