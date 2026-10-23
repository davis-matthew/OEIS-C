/*
 * A037736  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,1,0,3.
 * https://oeis.org/A037736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037736 – Decimal expansion of a(n) from the period 2,1,0,3 */
#include <stdlib.h>           /* malloc, realloc, free */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] unused; cache[1] = a(1) … */
    static idx_t cache_max = 0;      /* highest index stored in cache */

    /* already computed? */
    if (n <= cache_max && n > 0)
        return cache[n];

    /* grow cache to include n */
    idx_t new_size = (n > 1 ? n + 1 : 2);   /* at least 2 for n = 1 */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        return 0;                         /* allocation failed – unlikely for n ≤ 10000 */

    cache = new_cache;
    if (cache_max == 0)                   /* initialise unused slot 0 */
        cache[0] = 0;

    /* periodic digit table */
    static const seq_t digit_tbl[4] = { (seq_t)2, (seq_t)1, (seq_t)0, (seq_t)3 };

    /* compute missing values */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        seq_t d = digit_tbl[(i - 1) & 3];   /* period repeats every 4 */
        cache[i] = cache[i - 1] * (seq_t)10 + d;
    }
    cache_max = n;
    return cache[n];
}
