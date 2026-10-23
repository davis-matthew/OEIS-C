/*
 * A025275  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 5,
 *          with initial values 1,0,1,0.
 * https://oeis.org/A025275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)*a(n-1)+a(2)*a(n-2)+...+a(n-1)*a(1) for n >=5;
 *            Initial values: a(1)=1, a(2)=0, a(3)=1, a(4)=0
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;
/* Allocated capacity of the cache (in terms of seq_t elements) */
static size_t cap = 0;
/* Number of computed terms stored in the cache */
static size_t len = 0;

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;
    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    /* realloc returns void* in C; cast explicitly for C++ compatibility. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failed – terminate */
    cache = tmp;
    cap   = new_cap;
}

/* OEIS sequence A025275 */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; behaviour for n<1 is irrelevant */
        return 0;

    /* Initialise first four values on first use */
    if (len == 0) {
        ensure_capacity(4);
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)0;   /* a(2) */
        cache[2] = (seq_t)1;   /* a(3) */
        cache[3] = (seq_t)0;   /* a(4) */
        len = 4;
    }

    /* Return already known values directly */
    if ((size_t)n <= len)
        return cache[(size_t)n - 1];

    /* Expand the cache to the required size */
    ensure_capacity((size_t)n);

    /* Compute new terms sequentially */
    for (idx_t j = len + 1; j <= n; ++j) {
        seq_t sum = 0;
        for (idx_t k = 1; k < j; ++k) {
            /* indices k-1 and j-k-1 refer to stored terms */
            seq_t prod = cache[(size_t)(k - 1)] * cache[(size_t)(j - k - 1)];
            sum += prod;
        }
        cache[(size_t)j - 1] = sum;
    }

    len = (size_t)n;
    return cache[(size_t)n - 1];
}
