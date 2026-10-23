/*
 * A038761  a(n) = 6*a(n-1) - a(n-2), n >= 2, a(0)=1, a(1)=9.
 * https://oeis.org/A038761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 9, a(n) = 6*a(n-1) - a(n-2) for n >= 2 (OEIS
 *            A038761)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms               */
    static size_t  cap   = 0;     /* allocated capacity of the array        */
    static size_t  len   = 0;     /* number of terms already filled (len>=0)*/

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();          /* out of memory                 */
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute terms up to n (recurrence) */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)9;
        } else {
            cache[i] = (seq_t)6 * cache[i-1] - cache[i-2];
        }
    }

    len = (size_t)n + 1;          /* update number of filled terms */
    return cache[(size_t)n];
}
