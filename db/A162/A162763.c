/*
 * A162763  Minimal total number of floors an elevator must travel to get n
 *          people waiting, respectively, at floors i = 1, 2, ..., n, to their
 *          destinations at floors n+1-i (= n, ..., 1), if the elevator can
 *          hold at most C = 3 people and starts at floor 1, and no passenger
 *          may get off the elevator before reac...
 * https://oeis.org/A162763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A162763 – minimal total elevator travel with capacity 3.
   Base values (n = 1 … 9):
        a(1) = 0,  a(2) = 2,  a(3) = 4,  a(4) = 6,
        a(5) = 8 , a(6) =10, a(7) =12, a(8) =18, a(9) =22.
   For n ≥ 10 the recurrence holds:
        a(n) = 2*n + 1 + a(n-6).
   The function memoises computed values in a static array.
*/

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;   /* cache[0] unused, cache[i] = a(i) */
static idx_t   cap   = 0;     /* allocated size (directly) */

/* Ensure cache holds values up to index n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (n < cap)                      /* already enough */
        return;

    idx_t new_cap = n + 1;            /* indices 0 … n */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* out of memory */

    /* Copy old data if any */
    idx_t old_cap = cap;
    for (idx_t i = 0; i < old_cap; ++i)
        new_cache[i] = cache[i];

    /* Free old buffer */
    free(cache);
    cache = new_cache;
    cap   = new_cap;

    /* Initialise the new entries (indices old_cap … n) */
    for (idx_t i = old_cap; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
            continue;
        }
        if (i <= 7)                      /* a(n) = 2*n-2 for 1≤n≤7 */
            cache[i] = (seq_t)2 * (seq_t)i - 2;
        else if (i == 8)                  /* special values */
            cache[i] = (seq_t)18;
        else if (i == 9)
            cache[i] = (seq_t)22;
        else                              /* recurrence for n ≥ 10 */
            cache[i] = ((seq_t)2 * (seq_t)i + 1) + cache[i-6];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined, but return 0 */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
