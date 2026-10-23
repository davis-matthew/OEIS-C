/*
 * A176640  Partial sums of A005985.
 * https://oeis.org/A176640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A176640 – Partial sums of A005985
 * Recurrence (order 6): a(n) = 3*a(n-1)+3*a(n-2)-15*a(n-3)+6*a(n-4)+12*a(n-5)-8*a(n-6)
 * Initial values: a(0)=0, a(1)=1, a(2)=5, a(3)=14, a(4)=46, a(5)=111
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold at least "needed" elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise the first six terms on first use. */
    if (cache == NULL) {
        ensure_capacity(6);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)46;
        cache[5] = (seq_t)111;
        cache_size = 6;
    }

    if ((size_t)n < cache_size)            /* value already computed */
        return cache[(size_t)n];

    ensure_capacity((size_t)n + 1);

    /* Compute missing terms up to n. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t pos = cache[i - 1] * (seq_t)3;
        pos += cache[i - 2] * (seq_t)3;
        pos += cache[i - 4] * (seq_t)6;
        pos += cache[i - 5] * (seq_t)12;

        seq_t neg = cache[i - 3] * (seq_t)15;
        neg += cache[i - 6] * (seq_t)8;

        cache[i] = pos - neg;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
