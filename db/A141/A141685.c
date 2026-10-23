/*
 * A141685  a(1) = 1, a(n) = Sum_{k=1..n} (k mod 3) * a(n-k) for n >= 2.
 * https://oeis.org/A141685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k mod 3) * a(n-k) Initial values: a(1)=1,
 *            a(2)=1, a(3)=3, a(4)=5, a(0)=0 (used internally)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values a(0..cache_len) */
static idx_t cache_len = 0;      /* highest index that has been computed */

/* extend cache to include values up to `upto` (inclusive) */
static void ensure_cache(idx_t upto)
{
    if (cache_len >= upto) return;            /* already computed */

    /* enlarge storage */
    size_t new_cap = (size_t)upto + 1;         /* need elements [0..upto] */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                  /* unrecoverable allocation error */
    cache = new_cache;

    /* compute missing values */
    for (idx_t i = cache_len; i <= upto; ++i) {
        if (i == 0)                cache[i] = (seq_t)0;
        else if (i == 1 || i == 2) cache[i] = (seq_t)1;
        else if (i == 3)           cache[i] = (seq_t)3;
        else if (i == 4)           cache[i] = (seq_t)5;
        else                       cache[i] = cache[i-1] + (seq_t)2 * cache[i-2] + cache[i-3];
    }
    cache_len = upto;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* out‑of‑bounds calls are ignored */
    ensure_cache(n);                          /* guarantee all a(0..n) are computed */
    return cache[n];
}
