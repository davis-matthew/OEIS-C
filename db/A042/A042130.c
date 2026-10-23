/*
 * A042130  Numerators of continued fraction convergents to sqrt(590).
 * https://oeis.org/A042130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less, changed
 * Formula:   * a(n) = 11562 * a(n-6) - a(n-12) * Initial values: * a(0)=24,
 *            a(1)=73, a(2)=170, a(3)=753, a(4)=1676, a(5)=5781
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..11 */
    static const seq_t init[12] = {
        (seq_t)24, (seq_t)73, (seq_t)170, (seq_t)753,
        (seq_t)1676, (seq_t)5781,
        (seq_t)279164, (seq_t)843273, (seq_t)1965710,
        (seq_t)8706113, (seq_t)19377936, (seq_t)66839921
    };

    /* Cached terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  computed_until = 0;   /* first index NOT yet computed */

    /* Ensure capacity for up to 'n' */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)              /* allocation failure */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute all values from 'computed_until' up to 'n' */
    if (n >= computed_until) {
        for (idx_t i = computed_until; i <= n; ++i) {
            if (i < 12)                /* use the initial table */
                cache[i] = init[i];
            else                       /* recurrence  */
                cache[i] = cache[i-6] * (seq_t)11562 - cache[i-12];
        }
        computed_until = n + 1;        /* next index to compute */
    }

    return cache[n];
}
