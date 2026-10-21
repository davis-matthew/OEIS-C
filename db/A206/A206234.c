/*
 * A206234  Number of (n+1) X 5 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two equal edges, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A206234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206234
   a(n) = a(n-1) + 339*a(n-3) - 339*a(n-4) for n > 8
   a(1) = 1640       a(2) = 5728      a(3) = 27168
   a(4) = 283728     a(5) = 1236432  a(6) = 9042600
   a(7) = 95322432   a(8) = 419146392
   Recurrence applies for n = 9,10,11,…
   The function is fully memoised and thread‑safe only in the sense of
   static storage; concurrent calls may race but no side effects are
   performed beyond the static cache.
*/
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache holding values a(1), a(2), …  (cache[0] = a(1)) */
    static seq_t *cache = NULL;     /* cache[0] corresponds to a(1) */
    static idx_t cached_len = 0;    /* number of cached elements */

    /* Initial values for 1 <= n <= 8 */
    static const seq_t init[8] = {
        (seq_t)1640,
        (seq_t)5728,
        (seq_t)27168,
        (seq_t)283728,
        (seq_t)1236432,
        (seq_t)9042600,
        (seq_t)95322432,
        (seq_t)419146392
    };

    /* Allocate or resize cache to hold up to n entries */
    if (n > cached_len) {
        /* grow to n elements (size_t is large enough for 10 000) */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure: unlikely */
        cache = tmp;
        cached_len = n;
    }

    /* Ensure first 8 entries are initialised (they are identical on
       subsequent calls, so no harm). */
    idx_t first = (n < 8 ? n : 8);
    for (idx_t i = 0; i < first; ++i)
        cache[i] = init[i];

    /* If n <= 8 we already have the answer */
    if ((idx_t)n <= 8)
        return cache[n-1];

    /* Factor 339 as a seq_t once (saves casts). */
    seq_t factor = (seq_t)339;

    /* Compute remaining values using the recurrence. */
    for (idx_t i = 8; i < n; ++i) {    /* i is zero‑based, cache[i] = a(i+1) */
        seq_t t1 = factor * cache[i-3];
        seq_t t2 = factor * cache[i-4];
        seq_t sum = cache[i-1] + t1 - t2;
        cache[i] = sum;
    }

    return cache[n-1];
}
