/*
 * A042369  Denominators of continued fraction convergents to sqrt(711).
 * https://oeis.org/A042369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042369: Denominators of continued‑fraction convergents to √711
   Recurrence:
        a(0) = 1
        a(1) = 1
        for n ≥ 2 let r = (n‑1) & 3
          if r == 3      a(n) = 52*a(n‑1) + a(n‑2)
          else           a(n) = a(n‑1) + a(n‑2)
   The pattern of continued‑fraction quotients is
   (26, 1, 1, 1, 52, 1, 1, 1, 52, …), hence the recurrence above. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* array of a(n) values   */
static idx_t  cache_len = 0;       /* number of values stored (next free index) */
static size_t cache_cap = 0;       /* current malloc capacity */

/* Ensure that cache can hold indices up to `needed`.  */
static void ensure_cache_capacity(idx_t needed)
{
    /* Need space for indices 0 … needed, inclusive.  */
    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 4;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;

    if (new_cap == cache_cap)
        return; /* already sufficient */

#ifdef __cplusplus
    /* Allocate a new array and copy existing values.  */
    seq_t *new_cache = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete [] cache;
    cache = new_cache;
    cache_cap = new_cap;
#else
    /* Reallocate the existing C array.  */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
#endif
}

seq_t a(idx_t n)
{
    /* Base values.  */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Initialise cache if necessary.  */
    if (cache_len == 0) {
        ensure_cache_capacity(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Extend cache up to n if required.  */
    if (n >= cache_len) {
        ensure_cache_capacity(n);
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t ak = (((k - 1) & 3ULL) == 3) ? (seq_t)52 : (seq_t)1;
            cache[k] = ak * cache[k - 1] + cache[k - 2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
