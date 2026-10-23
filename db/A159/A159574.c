/*
 * A159574  Positive numbers y such that y^2 is of the form x^2+(x+337)^2 with
 *          integer x.
 * https://oeis.org/A159574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3)-a(n-6) for n>6; a(1)=313, a(2)=337, a(3)=365,
 *            a(4)=1513, a(5)=1685, a(6)=1877. This implementation uses a
 *            static memoization array, extending it as needed. Multiplication
 *            by 6 is performed in an unsigned 128‑bit integer to avoid
 *            overflow on 64‑bit types
 * Range:     u8 none, u16 n <= 12, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* cached terms; cache[0] unused */
static size_t cache_len = 0;      /* largest index currently stored */

/* Retrieve the n-th term of sequence a(n).  n >= 1. */
seq_t a(idx_t n)
{
    /* Initialize cache with the first six terms. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 7);
        if (!cache) abort(); /* allocation failed */
        cache[0] = 0;
        cache[1] = (seq_t)313;
        cache[2] = (seq_t)337;
        cache[3] = (seq_t)365;
        cache[4] = (seq_t)1513;
        cache[5] = (seq_t)1685;
        cache[6] = (seq_t)1877;
        cache_len = 6;
    }

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n];

    /* Resize cache to hold up to n. */
    size_t new_len = (size_t)n + 1;            /* array size needed */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        free(cache);
        abort();                            /* allocation failed */
    }
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        unsigned __int128 prod = (unsigned __int128)cache[i - 3] * 6U;
        cache[i] = (seq_t)(prod - cache[i - 6]);
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
