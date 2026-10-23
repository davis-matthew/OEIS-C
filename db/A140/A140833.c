/*
 * A140833  Sum of Fibonacci numbers between F(-n)....F(n), inclusive.
 * https://oeis.org/A140833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2, a(2)=2, a(3)=6; a(n)=3*a(n-2)-a(n-4) for n≥4
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed terms */
static size_t cap   = 0;      /* allocated capacity */
static idx_t  len   = 0;      /* number of terms already stored (len==index of last +1) */

/* Ensure that the cache can hold at least `needed` elements.
   Returns 0 on success, non‑zero on allocation failure. */
static int ensure_cache_size(size_t needed)
{
    if (cap >= needed)
        return 0;
    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
#if defined(__cplusplus)
    seq_t *tmp = static_cast<seq_t *>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp)   /* allocation failed */
        return -1;
    cache = tmp;
    cap   = new_cap;
    return 0;
}

/* Return the nth term of A140833 (sequence defined for n ≥ 0). */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold n */
    if (ensure_cache_size((size_t)n + 1) != 0)
        return (seq_t)0;   /* allocation failure – unlikely in the tests */

    /* Initialise base terms on first call */
    if (len == 0) {
        cache[0] = (seq_t)0;
        len = 1;
    }
    if (n < len)
        return cache[n];

    /* Fill in missing base terms if the cache is too small yet */
    if (len <= 1) {
        cache[1] = (seq_t)2;
        len = 2;
    }
    if (len <= 2) {
        cache[2] = (seq_t)2;
        len = 3;
    }
    if (len <= 3) {
        cache[3] = (seq_t)6;
        len = 4;
    }

    /* Compute terms using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = cache[i-2] * (seq_t)3 - cache[i-4];
    }
    len = n + 1;
    return cache[n];
}
