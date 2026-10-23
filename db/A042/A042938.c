/*
 * A042938  Reverse or triple: if reverse(a(n)) > a(n), a(n+1) =
 *          reverse(a(n)), else a(n+1) = 3*a(n).
 * https://oeis.org/A042938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   reverse, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>=1: a(n+1)=reverse(a(n)) if reverse(a(n))>a(n) else 3*a(n) */
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/reverse.h"

/* Static cache of computed values.  cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cap  = 0;   /* allocated capacity  (cache[0..cap-1])   */
static size_t len  = 0;   /* number of values stored (max index = len) */

/* Ensure that cache has room for indices 0 .. n  (i.e. capacity ≥ n+1) */
static void ensure_len(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cap) return;

    seq_t *new_cache;
    if (cap == 0) {
        new_cache = seq_alloc(need);
    } else {
        new_cache = seq_realloc(cache, cap, need);
    }
    if (!new_cache) return;  /* allocation failed – leave cache unchanged */

    cache = new_cache;
    cap   = need;
}

/* Return the n‑th term of A042938 (n ≥ 1).  For n < 1 return 0. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1 */

    ensure_len(n);

    /* Lazily initialise the first term and the cache length. */
    if (len == 0) {
        cache[1] = 1;   /* a(1) = 1 */
        len      = 1;
    }

    /* Compute any missing terms up to n.  The loop starts at len+1. */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse(prev);
        if (rev > prev)
            cache[i] = rev;
        else
            cache[i] = prev * 3;
    }
    len = (size_t)n;

    return cache[n];
}
