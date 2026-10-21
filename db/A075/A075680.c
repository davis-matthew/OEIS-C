/*
 * A075680  For odd numbers 2n-1, the minimum number of iterations of the
 *          reduced Collatz function R required to yield 1. The function R is
 *          defined as R(k) = (3k+1)/2^r, with r as large as possible.
 * https://oeis.org/A075680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1781, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A075680
   a(1) = 0
   a(n) = a(m) + 1   for n > 1
   where k = 2n‑1 is the odd number at index n,
   m = (R(k) + 1)/2,
   and R(k) = (3k+1)/2^r with r chosen maximal (i.e. shift out all trailing zeros).
   Consequently, a(n) is the number of reduced Collatz iterations needed to
   reduce the odd number 2n‑1 to 1. */

#include <stdlib.h>

/* Cache for a(n).  All indices are 1‑based. */
static seq_t *cache_steps = NULL;       /* a(n) values  */
static unsigned char *cache_known = NULL; /* 1 = cached, 0 = unknown */
static idx_t cache_cap = 0;               /* allocated size */

/* Allocate / enlarge cache to at least size n+1.  Newly allocated slots are
   marked unknown. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < n + 1)
        new_cap = n + 1;

    cache_steps = (seq_t *)realloc(cache_steps, new_cap * sizeof(seq_t));
    cache_known = (unsigned char *)realloc(cache_known,
                                           new_cap * sizeof(unsigned char));
    for (idx_t i = cache_cap; i < new_cap; ++i)
        cache_known[i] = 0; /* unknown */
    cache_cap = new_cap;
}

/* Recursive evaluation with memoisation.  The first call for any index
   1 < n <= cache_cap will trigger ensure_cache for the reachable index
   m, so requests for indices larger than the current cache are handled. */
static seq_t rec_a(idx_t n)
{
    if (n == 1)          /* a(1) = 0 */
        return 0;
    if (cache_known[n])
        return cache_steps[n];

    /* Map index n to the corresponding odd number k = 2n‑1. */
    seq_t k = (seq_t)2 * n - 1;          /* odd */

    /* Compute R(k): 3k+1 and shift out all trailing zeros. */
    seq_t next = 3 * k + 1;              /* 3k+1 is even */
    while ((next & 1) == 0)
        next >>= 1;                      /* take out powers of 2 */

    /* The odd number returned by R(k) is 'next'; its index m = (next+1)/2. */
    idx_t m = (idx_t)((next + 1) >> 1);
    ensure_cache(m);

    seq_t val = rec_a(m) + 1;
    cache_steps[n] = val;
    cache_known[n] = 1;
    return val;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return rec_a(n);
}
