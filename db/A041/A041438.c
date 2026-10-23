/*
 * A041438  Numerators of continued fraction convergents to sqrt(235).
 * https://oeis.org/A041438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041438: numerators of convergents for sqrt(235)
   Recurrence: a(0)=15, a(1)=46, a(2)=1395, a(3)=4231
                a(n) = 92*a(n-2) - a(n-4)   for n ≥ 4
   This implements a cache to allow arbitrary order of calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of terms stored */

    /* grow cache to hold term n (index starts at 0) */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)15;
            } else if (i == 1) {
                cache[1] = (seq_t)46;
            } else if (i == 2) {
                cache[2] = (seq_t)1395;
            } else if (i == 3) {
                cache[3] = (seq_t)4231;
            } else {
                /* recurrence: a(n) = 92*a(n-2) - a(n-4) */
                seq_t term = (seq_t)92 * cache[i-2] - cache[i-4];
                cache[i] = term;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
