/*
 * A137294  A polynomial-time algorithm for moving all seeds into one pot in a
 *          class of sowing positions.
 * https://oeis.org/A137294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 0
 *   a(2m)   = 3 * a(m)   + 1
 *   a(2m+1) = 2 * a(m) + a(m+1) + 2
 *
 *  Computes the sequence A137294 as defined by the recurrence above.
 *  The function is memoised in static storage so that repeated calls
 *  with arbitrary argument order are O(log n) each after the first call.
 */

#include <stdlib.h>
#include <string.h>   /* for memset */

/* Forward declaration of the recursive helper */
static seq_t compute_recursive(idx_t n);

/* Static cache of computed values and a flag array */
static seq_t  *cache        = NULL;  /* cache[n] holds a(n) once computed */
static char   *computed     = NULL;  /* 1 if a(n) has been computed */
static idx_t   cache_cap    = 0;      /* current capacity of the arrays */

/* Ensure that the cache can store values up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = n + 1;          /* we need indices 0..n inclusive */

    /* Reallocate both arrays to the larger size */
    seq_t  *new_cache   = (seq_t *)realloc(cache,   new_cap * sizeof(seq_t));
    char   *new_computed= (char  *)realloc(computed,new_cap * sizeof(char));

    if (!new_cache || !new_computed) {
        /* Memory allocation failed – terminate the program.
         * This should never happen in the typical use cases of the library. */
        abort();
    }

    /* Initialise the new portions of the arrays to zero */
    if (cache_cap < new_cap) {
        memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        memset(new_computed + cache_cap, 0, (new_cap - cache_cap) * sizeof(char));
    }

    cache     = new_cache;
    computed  = new_computed;
    cache_cap = new_cap;
}

/* Recursive helper that assumes ensure_capacity(n) has already been called. */
static seq_t compute_recursive(idx_t n)
{
    if (computed[n])
        return cache[n];

    seq_t val;
    if (n == 1) {
        val = 0;
    } else if ((n & 1) == 0) {          /* n even: n = 2m   */
        idx_t m = n >> 1;                /* n / 2          */
        val = 3 * compute_recursive(m) + 1;
    } else {                            /* n odd: n = 2m+1 */
        idx_t m = (n - 1) >> 1;          /* floor(n/2)     */
        val = 2 * compute_recursive(m) + compute_recursive(m + 1) + 2;
    }

    cache[n]    = val;
    computed[n] = 1;
    return val;
}

/* External linkage function required by the builder. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* Undefined for n < 1 – return 0 as a safe default. */

    ensure_capacity(n);
    return compute_recursive(n);
}
