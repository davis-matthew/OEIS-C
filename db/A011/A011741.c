/*
 * A011741  A binary m-sequence: expansion of reciprocal of x^28 + x^3 + 1
 *          (mod 2, shifted by 27 initial 0's).
 * https://oeis.org/A011741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0..26) = 0, a(27) = 1;
   a(n) = a(n-3) XOR a(n-28) for n >= 28. */
#ifdef __cplusplus
#  define NEW_ARRAY(n) new seq_t[(n)]
#  define DELETE_ARRAY(p) delete[] (p)
#else
#  include <stdlib.h>
#  define NEW_ARRAY(n) ((seq_t*)malloc((n)*sizeof(seq_t)))
#  define DELETE_ARRAY(p) free((p))
#endif

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t size = 0;      /* number of elements currently stored */
    static size_t cap  = 0;      /* allocated capacity */

    /* handle the trivial first 27 terms (always 0) and the first 1 */
    if (n < 27U) return (seq_t)0;
    if (n == 27U) return (seq_t)1;

    /* ensure the cache has enough capacity */
    size_t needed = (size_t)n + 1U;
    if (needed > cap)
    {
        size_t new_cap = needed * 2U;
        seq_t *new_cache = NEW_ARRAY(new_cap);
        if (!new_cache) abort();  /* out of memory */
        /* copy old entries if present */
        if (cache)
        {
            for (size_t i = 0; i < size; ++i)
                new_cache[i] = cache[i];
        }
        /* free old storage */
        if (cache) DELETE_ARRAY(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* initialise the first 28 entries on the first call */
    if (size < 28U)
    {
        for (size_t i = size; i < 28U; ++i)
            cache[i] = (i == 27U) ? (seq_t)1 : (seq_t)0;
        size = 28U;
    }

    /* extend the cache up to n using the recurrence */
    for (size_t i = size; i <= (size_t)n; ++i)
    {
        cache[i] = cache[i-3U] ^ cache[i-28U];
    }
    size = (size_t)n + 1U;

    return cache[(size_t)n];
}
