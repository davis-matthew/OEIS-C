/*
 * A021810  Decimal expansion of 1/806.
 * https://oeis.org/A021810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n-th decimal digit of 1/806.  The digit is
   obtained by performing long division of 1 by 806, keeping a running
   remainder.  A simple cache stores already-computed digits to avoid
   recomputation on subsequent calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for digits already computed */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of the cache array   */
    static size_t sz    = 0;   /* number of digits stored       */
    static idx_t rem    = 1;   /* remainder after last digit    */

    /* grow the cache if necessary */
    if (n >= sz) {
        size_t needed = (size_t)(n + 1);
        if (needed > cap) {
            /* increase capacity exponentially */
            size_t new_cap = (cap == 0) ? 1 : cap << 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        /* compute digits from the end of the current cache up to n */
        for (size_t i = sz; i <= n; ++i) {
            idx_t prod  = rem * 10;          /* multiply remainder by 10 */
            seq_t digit = (seq_t)(prod / 806);/* next decimal digit   */
            cache[i]    = digit;
            rem = prod % 806;                /* update remainder     */
        }
        sz = n + 1;                           /* new size of cache   */
    }
    return cache[n];
}
