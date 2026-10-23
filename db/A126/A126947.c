/*
 * A126947  The sequence c[n] defined in A126940.
 * https://oeis.org/A126947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126947 – c[n] defined in A126940
   Recurrence:
     a(0)=0,  a(1)=2,  a(2)=4,  a(3)=12,  a(4)=42,  a(5)=132
     a(n)=2*a(n‑1)+a(n‑2)+7*a(n‑3)+4*a(n‑4)+4*a(n‑5)+a(n‑6)  (n≥6)   */

#include <stdlib.h>

/*  Static cache of already computed terms.  */
static seq_t *cache = NULL;          /* pointer to the array of values         */
static size_t cache_len = 0;         /* number of already filled elements      */

/*  Ensure that cache holds values up to and including index n.  */
static void build_up_to(idx_t n)
{
    if (n < (idx_t)cache_len)            /* already enough computed      */
        return;

    size_t target = (size_t)n + 1;        /* need indices 0 .. n          */

    /* Allocate new buffer.  The size is small enough for all tested n.   */
    seq_t *new_cache = (seq_t *)malloc(target * sizeof(seq_t));
    if (new_cache == NULL)                /* allocation failure            */
        abort();

    /* Copy existing values into the new buffer.  */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Compute missing terms.  */
    for (size_t i = cache_len; i < target; ++i) {
        if (i == 0)          new_cache[i] = (seq_t)0;
        else if (i == 1)     new_cache[i] = (seq_t)2;
        else if (i == 2)     new_cache[i] = (seq_t)4;
        else if (i == 3)     new_cache[i] = (seq_t)12;
        else if (i == 4)     new_cache[i] = (seq_t)42;
        else if (i == 5)     new_cache[i] = (seq_t)132;
        else {
            /* recurrence for i >= 6 */
            new_cache[i] =
                (seq_t)2 * new_cache[i-1]
              + new_cache[i-2]
              + (seq_t)7 * new_cache[i-3]
              + (seq_t)4 * new_cache[i-4]
              + (seq_t)4 * new_cache[i-5]
              + new_cache[i-6];
        }
    }

    /* Replace the old cache.  */
    free(cache);
    cache = new_cache;
    cache_len = target;
}

/*  Public function required by the evaluation harness.  */
seq_t a(idx_t n)
{
    build_up_to(n);
    return cache[(size_t)n];
}
