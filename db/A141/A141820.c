/*
 * A141820  a(1)=1. a(n) = the smallest integer > a(n-1) that is divisible by
 *          the number of 1's in the binary representation of n.
 * https://oeis.org/A141820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; for n>1, a(n) is the smallest integer > a(n-1) that is
 *            divisible by the number of 1’s in the binary representation of n
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Grow an array to at least “needed” elements, doubling each time. */
static size_t grow_to(size_t current, size_t needed)
{
    size_t new_cap = current;
    while (new_cap < needed)
        new_cap = (new_cap == 0 ? 16 : new_cap * 2);
    return new_cap;
}

/* Memoisation storage */
static seq_t *cache   = NULL; /* array of computed values, 0‑based index = n‑1 */
static size_t cap    = 0;     /* allocated capacity                         */
static size_t len    = 0;     /* number of values already computed          */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* out‑of‑offset – behaviour irrelevant */
        return (seq_t)0;

    /* Ensure the cache contains a(1)…a(n) */
    if ((size_t)n > len)
    {
        size_t need = (size_t)n;
        if (need > cap)
        {
            size_t new_cap = grow_to(cap, need);
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) /* allocation failure – give up */
            {
                free(cache);
                cache = NULL;
                cap = len = 0;
                return (seq_t)0;
            }
            cache = tmp;
            cap = new_cap;
        }

        /* Compute the missing terms */
        for (size_t i = len; i < need; ++i)   /* i = n-1 (0‑based) */
        {
            if (i == 0)                      /* a(1) */
            {
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t prev = cache[i - 1];         /* a(n-1) */
            unsigned pc = __builtin_popcountll((uint64_t)(i + 1)); /* popcount(n) */

            /* smallest integer > prev divisible by pc */
            seq_t rem = prev % (seq_t)pc;
            seq_t inc = (rem == 0) ? (seq_t)pc : ((seq_t)pc - rem);
            cache[i] = prev + inc;
        }
        len = need;
    }

    return cache[n - 1];
}
