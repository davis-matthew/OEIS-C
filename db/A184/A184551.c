/*
 * A184551  Super-birthdays (falling on the same weekday), version 3 (birth
 *          within 2 and 3 years after a February 29).
 * https://oeis.org/A184551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A184551 – Super‑birthdays (falling on the same weekday)
   Recurrence: a(n) = a(n-1) + a(n-4) – a(n-5),  n ≥ 5
   Initial values: a(0)=0, a(1)=11, a(2)=17, a(3)=22, a(4)=28
*/
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache for computed terms   */
static size_t  cache_cap  = 0;    /* allocated cache size       */
static size_t  max_cached = 0;    /* highest index that has a value */

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;            /* double until sufficient */

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq_cache = tmp;
    cache_cap = new_cap;

    /* Initialise the first five values on the first allocation or
       when they have not yet been set. */
    if (max_cached < 5) {
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)11;
        seq_cache[2] = (seq_t)17;
        seq_cache[3] = (seq_t)22;
        seq_cache[4] = (seq_t)28;
        max_cached = 4;
    }
}

seq_t a(idx_t n)
{
    /* Ensure that indices 0 … n are stored in the cache. */
    size_t idx = (size_t)n;
    ensure_cache_cap(idx + 1);

    /* The first five terms are already in the cache. */
    if (idx <= 4)
        return seq_cache[idx];

    /* Compute missing terms in increasing order up to n. */
    size_t start = max_cached + 1;
    if (start < 5)          /* safety if first call was with n<5 */
        start = 5;

    for (size_t i = start; i <= idx; ++i) {
        seq_t val = seq_cache[i - 1];
        val += seq_cache[i - 4];
        val -= seq_cache[i - 5];
        seq_cache[i] = val;
    }
    max_cached = idx;
    return seq_cache[idx];
}
