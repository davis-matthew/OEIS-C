/*
 * A015609  a(n) = 11*a(n-1) + 12*a(n-2).
 * https://oeis.org/A015609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 12*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
   Memoized linear recurrence implementation.
   The function can be called in any order; it maintains a
   static cache that expands as needed.
*/
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* static cache */
    static seq_t *cache = NULL;      /* array of computed terms   */
    static idx_t   sz     = 0;        /* number of terms stored    */
    static size_t  cap    = 0;        /* capacity of the array     */

    /* first allocation: store the first two terms */
    if (!cache) {
        cap = 4;                       /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        sz = 2;
    }

    /* if the term is already computed, return it */
    if (n < sz) return cache[n];

    /* ensure we have enough room */
    size_t needed = (size_t)(n + 1);
    if (cap < needed) {
        size_t new_cap = cap == 0 ? 4 : cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute new terms up to n */
    for (idx_t i = sz; i <= n; ++i) {
        cache[i] = 11 * cache[i - 1] + 12 * cache[i - 2];
    }
    sz = n + 1;

    return cache[n];
}
