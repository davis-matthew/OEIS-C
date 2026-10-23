/*
 * A126477  Number of base 9 n-digit numbers with adjacent digits differing by
 *          three or less.
 * https://oeis.org/A126477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0)=1,  a(1)=9,  a(2)=51,  a(3)=301,  a(4)=1787
 *   a(n)=7*a(n-1)-6*a(n-2)-2*a(n-3)+2*a(n-4)  for n>4
 * This counts base‑9 n‑digit numbers whose adjacent digits differ by at most 3. */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* static cache of already computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Fast return if already computed */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to hold the requested term */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache) {
        /* Allocation failed – terminate */
        exit(EXIT_FAILURE);
    }

    /* Compute any newly needed terms */
    for (idx_t i = cache_len; i < new_len; ++i) {
#ifdef __cplusplus
        /* construct a seq_t object in the raw storage when building in C++ */
        new((void *)(cache + i)) seq_t();
#endif
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)9;
        else if (i == 2)
            cache[i] = (seq_t)51;
        else if (i == 3)
            cache[i] = (seq_t)301;
        else if (i == 4)
            cache[i] = (seq_t)1787;
        else {
            /* recurrence: a(n)=7*a(n-1)-6*a(n-2)-2*a(n-3)+2*a(n-4) */
            seq_t t1 = (seq_t)7 * cache[i-1];
            seq_t t2 = (seq_t)6 * cache[i-2];
            seq_t t3 = (seq_t)2 * cache[i-3];
            seq_t t4 = (seq_t)2 * cache[i-4];
            cache[i] = t1 - t2 - t3 + t4;
        }
    }
    cache_len = new_len;
    return cache[n];
}
