/*
 * A206901  Number of nonisomorphic graded posets with 0 of rank n with no
 *          3-element antichain.
 * https://oeis.org/A206901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A206901:
 * Recurrence:  a(n) = 7*a(n-1) - 10*a(n-2) + 3*a(n-3),
 * with base values a(0)=1, a(1)=2, a(2)=8. */

#include <stdlib.h>
#include <stddef.h>

/* Forward declaration of a helper function that ensures the cache
   has at least 'need' elements. */
static void ensure_capacity(size_t need);

/* static state for memoisation */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;   /* allocated size (elements) */
static size_t computed   = 0;   /* number of elements that have been
                                  computed (i.e. indices 0 .. computed-1) */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    /* Allocate more memory.  realloc(NULL, size) behaves like malloc. */
    cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!cache)
        abort();          /* abort on allocation failure */

    cache_cap = need;
}

/* ANSI C / C++ compatible implementation of the sequence. */
seq_t a(idx_t n)
{
    /* Initialise base cases if not already done. */
    if (computed == 0) {
        ensure_capacity(3);          /* indices 0..2 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        computed = 3;                /* indices 0..2 are valid now */
    }

    /* If the requested term has already been computed, return it. */
    if (n < (idx_t)computed)
        return cache[(size_t)n];

    /* Extend the cache to hold index n. */
    if ((size_t)n + 1 > cache_cap)
        ensure_capacity((size_t)n + 1);

    /* Compute missing terms using the recurrence. */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        /* Use seq_t arithmetic.  Constants are cast to seq_t to avoid
           accidental promotion to a larger type. */
        cache[i] = (seq_t)7 * cache[i - 1]
                 - (seq_t)10 * cache[i - 2]
                 + (seq_t)3 * cache[i - 3];
    }

    computed = (size_t)n + 1;          /* now all indices up to n are valid */
    return cache[(size_t)n];
}
