/*
 * A062063  a(n) is the smallest number greater than a(n-1) that is coprime to
 *          the last four terms of the sequence, with a(1) = 1, a(2) = 2, a(3)
 *          = 3, and a(4) = 5.
 * https://oeis.org/A062063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062063:  a(1)=1, a(2)=2, a(3)=3, a(4)=5.  
 * For n>4, a(n) is the smallest integer > a(n‑1) that is coprime
 * to the four preceding terms a(n‑1), a(n‑2), a(n‑3), a(n‑4).   */

#include <stddef.h>
#include <stdlib.h>

/* Euclidean GCD for the type seq_t.  Works for all unsigned integer
 * sizes (8, 16, 32, 64 or 128 bits). */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cache of already computed terms.  Index 0 is unused. */
static seq_t *cache      = NULL;  /* cache array */
static size_t cache_size = 0;     /* one past the highest stored index */

/* Return the n‑th term of A062063 (n >= 1).  */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;            /* target index (>=1) */

    /* Initialise the cache on first call.  */
    if (cache == NULL) {
        /* indices 0..4 : cache[0] unused, cache[1..4] given by definition. */
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;      /* out of memory */
        cache[0] = 0;                     /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
    }

    /* If the requested term is already cached, return it. */
    if (target < cache_size) {
        return cache[target];
    }

    /* Grow the cache so that index 'target' exists. */
    size_t new_size = target + 1;          /* +1 because indices start at 0 */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) return 0;             /* out of memory */
    cache        = tmp;
    size_t old_last = cache_size - 1;      /* last index previously stored */
    cache_size    = new_size;

    /* Compute missing terms in order. */
    for (size_t i = old_last + 1; i <= target; ++i) {
        seq_t a1 = cache[i - 1];  /* a(n-1) */
        seq_t a2 = cache[i - 2];  /* a(n-2) */
        seq_t a3 = cache[i - 3];  /* a(n-3) */
        seq_t a4 = cache[i - 4];  /* a(n-4) */

        seq_t candidate = a1 + 1;                 /* > a(n-1) */
        while (   gcd_seq_t(candidate, a1) != 1
               || gcd_seq_t(candidate, a2) != 1
               || gcd_seq_t(candidate, a3) != 1
               || gcd_seq_t(candidate, a4) != 1) {
            ++candidate;
        }
        cache[i] = candidate;
    }

    return cache[target];
}
