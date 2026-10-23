/*
 * A169502  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169502: number of reduced words of length n in the Coxeter
    group on 9 generators with relations S_i^2 = (S_iS_j)^34 = I.
    The sequence satisfies the linear recurrence derived from the
    generating function

          G(t) = (1 + 2t + … + 2t^33 + t^34) /
                 (1 – 7t – … – 7t^33 + 28t^34)

    i.e.

          a(n) = 7 * (a(n-1)+…+a(n-33)) – 28 * a(n-34) + c(n)

    where

          c(0) = 1,
          c(n) = 2   (1 ≤ n ≤ 33),
          c(34) = 1,
          c(n) = 0   (n ≥ 35).

    The implementation uses a static cache so that calls in any order
    are O(1) after the first full computation.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---- static cache ------------------------------------------- */
    static seq_t *cache = NULL;          /* array of computed terms   */
    static idx_t  cache_size = 0;        /* size of allocated array   */
    static idx_t  next = 0;              /* next index that must be
                                           recomputed            */

    /* ---- grow the cache if needed ----------------------------- */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory → zero   */
        cache = tmp;
        cache_size = n + 1;
    }

    /* ---- compute missing terms -------------------------------- */
    for (idx_t i = next; i <= n; ++i) {
        seq_t sum = 0;                     /* sum a(i-1) … a(i-33)   */

        /* add the 33 previous terms (if they exist) */
        for (idx_t k = 1; k <= 33; ++k) {
            if (i >= k) sum += cache[i - k];
        }

        /* start building a(i)  */
        seq_t term = (seq_t)7 * sum;

        /* -28 * a(i-34) if i ≥ 34 */
        if (i >= 34) term -= (seq_t)28 * cache[i - 34];

        /* plus the numerator coefficient c(i) */
        if (i == 0)       term += (seq_t)1;
        else if (i <= 33) term += (seq_t)2;
        else if (i == 34) term += (seq_t)1;
        /* otherwise c(i) = 0, nothing to add */

        cache[i] = term;
    }

    next = n + 1;          /* all indices < next are now computed   */
    return cache[n];
}
