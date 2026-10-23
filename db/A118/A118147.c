/*
 * A118147  Start with 1 and repeatedly reverse the digits and add 50 to get
 *          the next term.
 * https://oeis.org/A118147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(1) = 1, for n > 1: a(n) = reverse_decimal(a(n-1)) + 50
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array holding computed terms          */
    static size_t cap   = 0;        /* allocated capacity in elements         */
    static idx_t  computed = 0;      /* number of terms currently computed     */

    if (n < 1)
        return 0;                     /* undefined for n < 1, but safe return */

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        cap = 16;                     /* initial capacity */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
    }
    if ((idx_t)cap < n) {
        size_t newcap = cap;
        while ((idx_t)newcap < n) newcap <<= 1;   /* double until enough   */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* If the requested term is already known, return it */
    if (n <= computed)
        return cache[n-1];

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t next;
        if (i == 1) {
            next = 1;                 /* a(1) = 1 */
        } else {
            seq_t prev = cache[i-2];   /* a(i-1) already computed */
            next = reverse_decimal(prev) + (seq_t)50;
        }
        cache[i-1] = next;
    }

    computed = n;
    return cache[n-1];
}
