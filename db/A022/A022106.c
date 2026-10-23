/*
 * A022106  Fibonacci sequence beginning 1, 16.
 * https://oeis.org/A022106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0)=1, a(1)=16
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A022106, 0‑based.   */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache      = NULL; /* array of seq_t */
static idx_t cache_len   = 0;    /* number of elements stored   */
static idx_t cache_cap   = 0;    /* capacity of the buffer       */

/* Ensure that the cache can store at least 'needed' elements
 * (indices 0 .. needed-1).  The argument 'needed' is an upper bound
 * on the maximum index that must be stored, so we need capacity > needed. */
static void ensure_capacity(idx_t needed)
{
    if (needed < cache_cap)
        return;                            /* already ready          */

    idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= needed)
        new_cap <<= 1;                     /* grow until it fits     */

    /* realloc can be used with a null pointer; cast is required for C++ */
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_ptr) {                         /* success: update cache  */
        cache      = new_ptr;
        cache_cap  = new_cap;
    } /* if realloc fails we keep the old cache */
}

/* Return the nth term of sequence A022106 (0‑based). */
seq_t a(idx_t n)
{
    /* Initialise base case values on first call. */
    if (cache_len == 0) {
        ensure_capacity(2);                /* need indices 0 and 1   */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)16;
        cache_len = 2;
    }

    /* Extend the cache if we require a value not yet computed. */
    if (n >= cache_len) {
        ensure_capacity(n + 1);            /* store indices 0..n     */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i >= 2)
                cache[i] = cache[i - 1] + cache[i - 2];
            /* values for i==0 or 1 are already set                     */
        }
        cache_len = n + 1;
    }

    return cache[n];
}
