/*
 * A037525  Base-8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0.
 * https://oeis.org/A037525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=2, a(2)=17, a(3)=136, a(4)=1090 Recurrence: a(n) = 8*a(n-1)
 *            + a(n-3) - 8*a(n-4) (for n >= 5)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;      /* cache[0] = a(1), cache[1] = a(2), ... */
static size_t cache_size = 0;    /* number of terms currently cached */
static size_t cache_cap  = 0;    /* allocated capacity */

/* Ensure the cache can hold at least 'needed' terms (needed >= cache_size). */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

#ifdef __cplusplus
    /* Allocate new array with constructors called. */
    seq_t *newptr = new (std::nothrow) seq_t[new_cap];
    if (!newptr)
        abort(); /* unrecoverable: out of memory */

    /* Copy existing elements to the new array. */
    for (size_t i = 0; i < cache_size; ++i)
        newptr[i] = cache[i];

    /* Delete old array. */
    delete[] cache;
    cache = newptr;
#else
    /* Allocate with realloc (no constructor calls needed for built‑in types). */
    seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newptr)
        abort(); /* unrecoverable: out of memory */
    cache = newptr;
#endif
    cache_cap = new_cap;
}

/* Return the n-th term of A037525 (a(1)=2). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for OEIS, but safe */

    size_t needed = (size_t)n;  /* n fits easily in size_t for n up to ~10 000 */

    if (needed > cache_size) {
        ensure_capacity(needed);

        for (size_t i = cache_size; i < needed; ++i) {
            if (i == 0)
                cache[i] = (seq_t)2;
            else if (i == 1)
                cache[i] = (seq_t)17;
            else if (i == 2)
                cache[i] = (seq_t)136;
            else if (i == 3)
                cache[i] = (seq_t)1090;
            else
                /* recurrence: a(n) = 8*a(n-1) + a(n-3) - 8*a(n-4) */
                cache[i] = (seq_t)8 * cache[i - 1]
                         + cache[i - 3]
                         - (seq_t)8 * cache[i - 4];
        }
        cache_size = needed;
    }

    return cache[n - 1];
}
