/*
 * A125678  a(0) = 1; for n>0, a(n) = (a(n-1)^2 reduced mod n) + 1.
 * https://oeis.org/A125678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = (a(n-1)^2 mod n) + 1
 * Range:     u8 n <= 17, u16 n <= 588, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation state */
static seq_t *cache   = NULL;   /* array of previously computed terms */
static size_t cap     = 0;      /* current capacity of cache (in elements) */
static idx_t max_calc = (idx_t)-1;  /* largest index computed so far */

/* Ensure cache has space for indices up to idx (inclusive) */
static void ensure_cache (idx_t idx)
{
    if (cap <= (size_t)idx) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc (cache, new_cap * sizeof (seq_t));
        if (!tmp) abort ();
        /* Zero any newly allocated slots (seq_t has a zero‑value constructor) */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }
}

/* Main sequence function */
seq_t a (idx_t n)
{
    /* Prepare storage */
    ensure_cache (n);

    if (max_calc == (idx_t)-1) {      /* first call: initialise a(0) */
        cache[0] = (seq_t)1;
        max_calc = 0;
    }

    if (n == 0) return cache[0];

    /* Compute missing terms up to n */
    if (max_calc < n) {
        for (idx_t i = max_calc + 1; i <= n; ++i) {
            seq_t prev   = cache[i - 1];
            seq_t mod_val = prev % (seq_t)i;
            seq_t sq      = mod_val * mod_val;
            cache[i] = (sq % (seq_t)i) + (seq_t)1;
        }
        max_calc = n;
    }

    return cache[n];
}
