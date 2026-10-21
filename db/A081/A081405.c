/*
 * A081405  a(n) = (n+1)*a(n-2) with a(0) = a(1) = 1.
 * https://oeis.org/A081405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-2), a(0)=a(1)=1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 32, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that the internal cache can hold at least 'needed' elements.  */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_len) return;

#ifdef __cplusplus
    /* Allocate a new C++ array, copy existing values and delete old one */
    seq_t *new_cache = new seq_t[needed];
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
    /* Zero the newly added part */
    for (idx_t i = cache_len; i < needed; ++i)
        cache[i] = 0;
#else
    /* Use realloc in C, zero any new slots */
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) abort();                  /* allocation failure */
    cache = tmp;
    for (idx_t i = cache_len; i < needed; ++i)
        cache[i] = 0;
#endif
    cache_len = needed;
}

/* Return the n-th term of A081405.  */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;

    ensure_capacity(n + 1);

    /* Base values understood to be 1 for all n. */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0) continue;      /* already computed */
        cache[i] = (seq_t)(i + 1) * cache[i - 2];
    }

    return cache[n];
}
