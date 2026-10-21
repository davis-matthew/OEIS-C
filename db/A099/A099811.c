/*
 * A099811  a(n) = a(n-1) XOR Sum_{k=1..n-1} a(k), with a(1)=1, a(2)=3, where
 *          XOR is the binary exclusive OR operation.
 * https://oeis.org/A099811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) XOR Sum_{k=1..n-1} a(k) (a(1)=1, a(2)=3)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based index, cache[0] unused */
    static size_t cap   = 0;      /* largest n for which cache is valid   */
    static seq_t sum    = 0;      /* sum of a(1)..a(n) for n = cap       */
    static seq_t last   = 0;      /* a(cap)                              */

    if (n == 0)                  /* undefined for n<1 – return 0 */
        return (seq_t)0;

    /* initialize the first two terms on first call */
    if (cap == 0) {
        cache   = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[1] = 1;
        cache[2] = 3;
        sum      = cache[1] + cache[2];
        last     = cache[2];
        cap      = 2;
    }

    if (n <= cap)
        return cache[n];

    /* extend cache to include index n */
    size_t prev_cap = cap;
    size_t new_cap  = n + 1;             /* +1 for the unused 0 index */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;           /* allocation failure (unlikely) */
    cache = tmp;
    cap   = n;

    /* compute missing terms starting from prev_cap+1 */
    for (idx_t i = prev_cap + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t val  = prev ^ sum;
        cache[i]   = val;
        sum += val;
    }

    return cache[n];
}
