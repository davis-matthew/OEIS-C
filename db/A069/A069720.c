/*
 * A069720  a(n) = 2^(n-1)*binomial(2*n-1, n).
 * https://oeis.org/A069720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 2^(n-1) * binomial(2*n-1, n) a(1) = 1 Recurrence: a(1) =
 *            1, n*a(n) = 4*(2*n-1)*a(n-1) for n > 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifndef SEQ_T
/* The build supplies typedefs for seq_t and idx_t (i.e. unsigned long long).  */
#endif

/* A simple memoised recurrence implementation.  The cache holds the
   values a[1..max_computed].  cache[0] is unused. */
static seq_t *cache = NULL;     /* array of seq_t, indexed from 0 (unused) */
static idx_t cache_size = 0;    /* highest index that is valid in cache */

/* Grow the cache so that it can hold values up to ‘n’.  cache_size
   becomes max(cache_size, n).  Returns NULL on allocation failure. */
static seq_t *ensure_cache(idx_t n)
{
    if (cache_size >= n) return cache;          /* already big enough */

    /* Allocate space for indices 0..n (index 0 is unused). */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return NULL;                /* allocation failed */

    /* Copy existing data. */
    for (idx_t i = 1; i <= cache_size; ++i) new_cache[i] = cache[i];
    /* Initialise new elements to zero (unset). */
    for (idx_t i = cache_size + 1; i <= n; ++i) new_cache[i] = (seq_t)0;

    free(cache);
    cache      = new_cache;
    cache_size = n;
    return cache;
}

/* ------------------------------------------------------------- */
/*    return a(n), with n >= 1  (a(0) is undefined, return 0)  */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* outside the offset */

    idx_t old_size = cache_size;              /* highest computed before */
    if (!ensure_cache(n)) return (seq_t)0;     /* allocation failure */

    /* Initialise a(1) if it is still unset. */
    if (cache[1] == 0) {
        cache[1] = (seq_t)1;
        if (old_size < 1) old_size = 1;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = old_size + 1; i <= n; ++i) {
        /* a(i) = (4*(2*i-1)/i) * a(i-1)   */
        idx_t mult  = 4 * (2 * i - 1);
        idx_t denom = i;

        seq_t val;
#ifdef __SIZEOF_INT128__
        /* Use a 128‑bit intermediate when seq_t is 64‑bit or smaller. */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 prod = (unsigned __int128)cache[i-1] * (unsigned __int128)mult;
            val = (seq_t)(prod / (unsigned __int128)denom);
        } else
#endif
        {
            /* For arbitrary‑precision seq_t, rely on its operators. */
            val = (cache[i-1] * (seq_t)mult) / (seq_t)denom;
        }

        cache[i] = val;
    }

    return cache[n];
}
