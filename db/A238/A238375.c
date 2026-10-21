/*
 * A238375  Row sums of triangle in A152719.
 * https://oeis.org/A238375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)-2*a(n-3)+a(n-4)-a(n-5) with initial
 *            values a(0)=1, a(1)=2, a(2)=4, a(3)=6, a(4)=11, a(5)=16
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 48, u64 n <= 99, u128 n <= 199,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamically allocated array for computed terms */
static size_t cap   = 0;       /* capacity of the allocated array */
static size_t len   = 0;       /* number of terms currently stored (cache[0..len-1]) */

/* Ensure the cache has room for indices 0..n (inclusive) */
static void ensure_cap(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cap >= need) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* out of memory – abort program (no recovery possible) */
        free(cache);
        cache = NULL;
        cap   = 0;
        len   = 0;
        exit(1);
    }
    cache = tmp;
    cap   = new_cap;
}

/* Sequence entry function */
seq_t a(idx_t n)
{
    /* Allocate sufficient space */
    ensure_cap(n);

    /* Initialise first six terms on first use */
    if (len == 0)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)11;
        cache[5] = (seq_t)16;
        len = 6;
    }

    /* If the requested term is already computed, return it */
    if ((size_t)n < len)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i)
    {
        cache[i] =
            cache[i-1]
          + ((seq_t)2 * cache[i-2])
          - ((seq_t)2 * cache[i-3])
          + cache[i-4]
          - cache[i-5];
    }

    len = (size_t)n + 1;    /* update count of stored terms */
    return cache[(size_t)n];
}
