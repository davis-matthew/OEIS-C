/*
 * A098576  a(n) = Sum_{k=0..floor(n/4)} C(n-2*k,2*k) * 3^k.
 * https://oeis.org/A098576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 70, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A098576
 *
 * Formula: a(n) = Sum_{k=0..floor(n/4)} C(n-2*k, 2*k) * 3^k
 *
 * Recurrence (used for fast calculation):
 *   a(0) = a(1) = a(2) = a(3) = 1
 *   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-4)   for n ≥ 4
 *
 * The implementation keeps a cached array of computed terms and expands it
 * as needed.  No external libraries are required beyond <stdlib.h>.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cached values: a_cache[i] == a(i)   */
static idx_t a_cache_size = 0;  /* number of cached entries          */

static void
ensure_cache(idx_t n)
{
    /* Nothing to do if the requested term is already cached. */
    if (n + 1 <= a_cache_size)
        return;

    /* Allocate a larger array to hold terms up to n. */
    idx_t new_size = n + 1;
    seq_t *new_cache;

#ifdef __cplusplus
    new_cache = reinterpret_cast<seq_t*>(realloc(a_cache, new_size * sizeof(seq_t)));
#else
    new_cache = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
#endif
    if (!new_cache) {
        /* Allocation failed – abort the program (no error handling defined). */
        abort();
    }

    /* Fill in new entries.  For n ≤ 3 the terms are 1; otherwise use the
     * recurrence, which uses only already-computed earlier terms. */
    for (idx_t i = a_cache_size; i < new_size; ++i) {
        if (i <= 3) {
            new_cache[i] = (seq_t)1;
        } else {
            seq_t term1 = (seq_t)2 * new_cache[i - 1];
            seq_t term2 = new_cache[i - 2];
            seq_t term3 = (seq_t)3 * new_cache[i - 4];
            new_cache[i] = term1 - term2 + term3;
        }
    }

    a_cache = new_cache;
    a_cache_size = new_size;
}

extern seq_t a(idx_t n)  /* external linkage as required */
{
    ensure_cache(n);
    return a_cache[n];
}
