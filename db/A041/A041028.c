/*
 * A041028  Numerators of continued fraction convergents to sqrt(19).
 * https://oeis.org/A041028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 4. Let period = {2,1,3,1,2,8}. For n ≥ 1: a(n) =
 *            period[(n-1)%6] * a(n-1) + a(n-2), with the convention a(-1) =
 *            1. This generates a(1)=9, a(2)=13, a(3)=48, … as in OEIS A041028
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached numerators.  All functions operate on seq_t and idx_t
   supplied by the build system. */
static seq_t *cache = NULL;      /* pointer to cached terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure we can store at least goal terms. */
static void
ensure_cache_cap(idx_t goal)
{
    size_t g = (size_t)goal;
    if (g <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < g) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* Out of memory.  Terminate. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A041028. */
seq_t
a(idx_t n)
{
    /* Periodic partial denominators of sqrt(19). */
    static const unsigned period[6] = {2, 1, 3, 1, 2, 8};

    /* Special case n==0. */
    if (n == 0) return (seq_t)4;

    /* Make sure the cache is large enough. */
    ensure_cache_cap(n + 1);

    /* Initialise cache if empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;
        cache_len = 1;
    }

    /* If the term is already cached, return it. */
    if (n < cache_len) return cache[n];

    /* Start recurrence from the first uncached index. */
    seq_t prevprev;  /* a(i-2) */
    seq_t prev;      /* a(i-1) */

    if (cache_len == 1) {   /* only a(0) stored */
        prevprev = (seq_t)1;          /* a(-1) = 1 */
        prev = cache[0];              /* a(0) */
    } else {                 /* at least a(1) stored */
        prevprev = cache[cache_len - 2];
        prev = cache[cache_len - 1];
    }

    /* Compute a(i) for i = cache_len … n. */
    size_t i;
    for (i = cache_len; (idx_t)i <= n; ++i) {
        unsigned factor = period[(i - 1) % 6];
        seq_t cur = (seq_t)factor * prev + prevprev;
        cache[i] = cur;
        prevprev = prev;
        prev = cur;
    }

    cache_len = n + 1;
    return cache[n];
}
