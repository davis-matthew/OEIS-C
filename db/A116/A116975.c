/*
 * A116975  Number of compositions of n into parts of sizes == 1 mod 5 or 4
 *          mod 5.
 * https://oeis.org/A116975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-4) + a(n-5)  (n ≥ 6)
   with initial values
       a(1) = 1
       a(2) = 1
       a(3) = 1
       a(4) = 2
       a(5) = 3
   This implements OEIS A116975.
   The function is memoised so that subsequent calls are O(1) after the
   first computation. */

#include <stdlib.h>
#include <stddef.h>

/* Global memoisation state */
static seq_t *cache = NULL;          /* array of computed a(i) values */
static size_t cache_len = 0;         /* current capacity (number of slots) */
static size_t max_computed = 0;      /* highest index whose value is set */

/* Ensure that cache has space for indices 0 ... needed (inclusive) */
static void ensure_cache(idx_t needed)
{
    size_t need = (size_t)needed + 1;      /* capacity needed */
    if (need <= cache_len)
        return;                           /* already large enough */

    size_t new_cap = cache_len ? cache_len * 2 : need;
    if (new_cap < need)
        new_cap = need;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                          /* allocation failure is fatal */

    /* Initialise newly allocated slots to 0 */
    for (size_t i = cache_len; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_len = new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* By definition n ≥ 1.  We keep a(0) = 0 for convenience. */
    if (n == 0)
        return 0;

    /* Make sure cache can hold index n. */
    ensure_cache(n);

    /* Initialise base values on first use. */
    if (max_computed < 5) {
        if (cache_len < 6)
            ensure_cache(5);

        cache[0] = 0; /* unused in the sequence */
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 2;
        cache[5] = 3;
        max_computed = 5;
    }

    /* If n is already computed, return it. */
    if ((size_t)n <= max_computed)
        return cache[n];

    /* Compute values up to n, using the recurrence. */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 4] + cache[i - 5];
    }
    max_computed = (size_t)n;
    return cache[n];
}
