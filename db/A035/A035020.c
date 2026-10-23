/*
 * A035020  One sixth of 9-factorial numbers.
 * https://oeis.org/A035020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*(n-1)*a(n-1) with a(1) = 1 * (equivalently a(n) =
 *            product_{j=1..n} (9*j-3) ). * This implementation memoizes
 *            values in a static array so that * calls with arbitrary indices
 *            are efficient. It works for any * seq_t (unsigned integer,
 *            arbitrary‑precision) and idx_t (uint64_t)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache array: cache[1] = a(1) */
static seq_t *cache = NULL;            /* array of seq_t values */
static idx_t cache_cap = 0;            /* current allocated size (>= 1) */
static idx_t computed = 0;             /* largest index for which a(i) is stored */

/* grow cache so that it can hold at least 'need' indices (0..need-1) */
static void grow_cache(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    /* On failure realloc returns NULL; let the program crash (usual practice). */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; 0 is outside the sequence */
        return (seq_t)0;

    /* ensure we have space for index n */
    if (n >= cache_cap)
        grow_cache(n + 1);     /* allocate indices 0 .. n */

    /* initialise a(1) if not already done */
    if (computed < 1 && cache[1] == (seq_t)0)
        cache[1] = (seq_t)1;   /* a(1) = 1 */

    /* compute missing terms up to n */
    idx_t start = computed < 1 ? 2 : computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t factor = (seq_t)9 * (seq_t)i - (seq_t)3; /* 9*i-3 */
        cache[i] = cache[i - 1] * factor;
    }
    if (n > computed)
        computed = n;

    return cache[n];
}
