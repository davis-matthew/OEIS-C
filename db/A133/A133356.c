/*
 * A133356  a(n) = 2*a(n-1) + 16*a(n-2) for n>1, a(0)=1, a(1)=1.
 * https://oeis.org/A133356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2), a(0)=1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#endif

seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache = NULL;          /* pointer to cached terms   */
    static idx_t  cap   = 0;             /* allocated capacity        */
    static idx_t  last  = 1;             /* highest index computed    */

    /* ---- initialisation -------------------------------------------------- */
    if (cache == NULL)
    {
#ifdef __cplusplus
        /* allocate two elements for the seed values */
        cap = 2;
        cache = new seq_t[cap];
        cache[0] = seq_t(1);
        cache[1] = seq_t(1);
#else
        cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)cap);
        if (!cache) return (seq_t)0;    /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
#endif
    }

    /* ---- grow cache if necessary --------------------------------------- */
    if (n >= cap)
    {
        idx_t new_cap = cap ? cap : 2;
        while (new_cap <= n) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        /* copy existing terms to the new array */
        for (idx_t i = 0; i <= last; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_cap);
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
#endif
        cap = new_cap;
    }

    /* ---- return if already cached -------------------------------------- */
    if (n <= last) return cache[n];

    /* ---- compute missing terms ---------------------------------------- */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* recurrence: a(i) = 2*a(i-1) + 16*a(i-2) */
        cache[i] = cache[i - 1] * ((seq_t)2) + cache[i - 2] * ((seq_t)16);
    }
    last = n;
    return cache[n];
}
