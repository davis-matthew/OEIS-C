/*
 * A017853  Expansion of 1/(1-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13).
 * https://oeis.org/A017853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13) for
 *            n>=14; a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1,
 *            a(7)=1, a(8)=1, a(9)=1, a(10)=1, a(11)=1, a(12)=2, a(13)=3
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 104, u64 n <= 199,
 *            u128 n <= 388, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence. */
static seq_t *cache       = NULL;
static size_t cache_alloc = 0;  /* allocated size of cache */
static size_t cache_len   = 0;  /* number of terms currently stored */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_alloc) return;

    size_t new_cap = cache_alloc ? cache_alloc * 2 : 1;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */

    cache       = tmp;
    cache_alloc = new_cap;
}

/* Return the n-th term of the sequence.  External linkage. */
seq_t a(idx_t n)
{
    /* Fast return if already cached. */
    if (n < (idx_t)cache_len)
        return cache[(size_t)n];

    /* Allocate cache space if needed. */
    ensure_capacity(n);

    /* Initial values for indices 0 .. 13. */
    static const seq_t init[14] = {
        (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3
    };

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        seq_t val;
        if (i <= 13)
            val = init[i];
        else
            val = cache[i-6] + cache[i-7] + cache[i-8] + cache[i-9]
                 + cache[i-10] + cache[i-11] + cache[i-12] + cache[i-13];
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
