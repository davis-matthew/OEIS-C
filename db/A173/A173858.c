/*
 * A173858  Expansion of 4/3 in base phi.
 * https://oeis.org/A173858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   G.f. = x*(1-x^2+x^3+x^4-x^6)/(1-x^2+x^4-x^6) Recurrence: a(n) =
 *            a(n-2) - a(n-4) + a(n-6) for n >= 8 Initial terms: a(1)=1,
 *            a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=1, a(7)=0 (the first 6
 *            terms also satisfy a(n) = A000475(n) mod 2)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of entries allocated (0..cache_size-1) */
    static idx_t computed_up_to = 0;  /* largest index for which a(i) has been computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 1;               /* at least index 0 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failed – return 0 as a safety */
        cache[0] = (seq_t)0;          /* unused slot at index 0 */
    }

    /* grow cache if needed and compute missing terms */
    if (n >= computed_up_to) {
        idx_t new_size = n + 1;
        if (new_size > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL) return 0;  /* allocation failed – return 0 */
            cache = tmp;
            cache_size = new_size;
        }

        /* compute terms from computed_up_to+1 up to n */
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            if (i == 1)              cache[i] = 1;
            else if (i == 2)         cache[i] = 0;
            else if (i == 3)         cache[i] = 0;
            else if (i == 4)         cache[i] = 1;
            else if (i == 5)         cache[i] = 0;
            else if (i == 6)         cache[i] = 1;
            else if (i == 7)         cache[i] = 0;
            else                     cache[i] = cache[i-2] - cache[i-4] + cache[i-6];
        }
        computed_up_to = n;
    }

    return cache[n];
}
