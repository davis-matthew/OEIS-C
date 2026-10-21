/*
 * A050028  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A050028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 195, u64 n <= 770,
 *            u128 n <= 3633, big any n
 * Helpers:   floor_log2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1, a(3)=2.
 * For n ≥ 4  :  a(n) = a(n‑1) + a(m)
 * where   m = 2·n – 3 – 2^(p+1)
 * and   p is the unique integer with 2^p < n–1 ≤ 2^(p+1).  
 *
 * The value of p is floor(log2(n‑1)) except when n-1 is an exact power
 * of two, in which case p = floor(log2(n‑1)) – 1.
 *
 * The sequence is memoised so that successive calls (in any order)
 * are fast.
 */

#include <stdlib.h>
#include "../utils/floor_log2.h"

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t  last_computed = 0;   /* highest n that has been filled */

/* initialise cache for n = 0..3 */
static void init_cache(void)
{
    /* allocate space for indices 0..3 */
    cache = (seq_t*)malloc(4 * sizeof(seq_t));
    cache[0] = (seq_t)0;   /* unused */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)2;
    last_computed = 3;
}

/* enlarge cache to at least size new_size (number of elements) */
static void ensure_cache(idx_t new_size)
{
    if (new_size <= last_computed) return;
    seq_t *new_cache = (seq_t*)realloc(cache, (new_size+1) * sizeof(seq_t));
    if (!new_cache) return;           /* failed allocation – ignore */
    cache = new_cache;
    /* initialise newly created entries to zero */
    for (idx_t i = last_computed + 1; i <= new_size; ++i) cache[i] = (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 */

    if (!cache) init_cache();

    if (n <= last_computed) return cache[n];

    /* ensure the cache is large enough */
    ensure_cache(n);

    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        /* m < i for all i ≥ 4, so a[i-1] and a[m] are ready */
        seq_t prev = cache[i-1];

        /* compute intermediate value of a(m) */
        idx_t u = i - 1;                 /* n-1 */
        idx_t l = floor_log2(u);          /* floor(log2(u)) */
        idx_t p = l;
        if ((idx_t)1 << l == u)
            --p;                          /* special case when u is a power of two */

        idx_t pow = (idx_t)1 << (p + 1);  /* 2^(p+1) */
        idx_t m   = (2 * i - 3) - pow;    /* m = 2n-3-2^(p+1) * /
                                          * m is guaranteed to be ≥ 1 and < i
                                          */
        seq_t val = prev + cache[m];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
