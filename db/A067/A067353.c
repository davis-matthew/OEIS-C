/*
 * A067353  Divide the natural numbers in sets of consecutive numbers starting
 *          with {1,2} as the first set. The number of elements of the n-th
 *          set is equal to the sum of the n-1 final numbers in the (n-1)st
 *          set. The final number of the n-th set gives a(n).
 * https://oeis.org/A067353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 2; a(n) = n*a(n-1) - (n-1)(n-2)/2 (for n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;     /* array of a[1..last] */
    static idx_t last = 0;          /* largest index computed so far */
    static idx_t cap = 0;           /* allocated capacity of cache */

    /* initial allocation */
    if (cache == NULL) {
        cap = (n < 2 ? 2 : n + 1);  /* at least space for index 1 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }

    /* grow if needed */
    if (n + 1 > cap) {
        idx_t new_cap = cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp) {
            cache = tmp;
            cap = new_cap;
        }
    }

    /* initialise first term if not yet done */
    if (last == 0) {
        cache[1] = (seq_t)2;
        last = 1;
    }

    /* compute terms up to n */
    while (last < n) {
        ++last;                         /* compute a[last] */

        seq_t prev = cache[last - 1];
        seq_t sub = ((seq_t)(last - 1) * (last - 2)) / (seq_t)2;
        cache[last] = (seq_t)last * prev - sub;
    }

    return cache[n];
}
