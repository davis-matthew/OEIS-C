/*
 * A188526  a(n) = 3^(2*n + 3) + 2^n.
 * https://oeis.org/A188526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(2*n + 3) + 2^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=28, a(1)=245, a(n)=11*a(n-1)-18*a(n-2) for n>=2 */

#include <stdlib.h>

/* static cache storing computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* ensure cache contains entries up to index n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;   /* already available */

    idx_t old_size = cache_size;               /* previous number of entries */
    size_t new_len   = (size_t)n + 1;           /* new number of entries */
    size_t new_bytes = new_len * sizeof(seq_t); /* new allocation size */

    if (cache == NULL) {
        cache = (seq_t*)malloc(new_bytes);
    } else {
        seq_t *tmp = (seq_t*)realloc(cache, new_bytes);
        if (tmp == NULL) abort();   /* allocation failure */
        cache = tmp;
    }

    /* initialise base terms if not already set */
    if (old_size <= 0 && n >= 0) {
        cache[0] = (seq_t)28;
    }
    if (old_size <= 1 && n >= 1) {
        cache[1] = (seq_t)245;
    }

    /* compute missing terms using the recurrence */
    idx_t start = (old_size > 2) ? old_size : 2;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)((seq_t)11 * cache[i-1] - (seq_t)18 * cache[i-2]);
    }

    cache_size = new_len;
}

/* public function returning a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
