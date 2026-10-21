/*
 * A054856  Number of ways to tile a 4 X n region with 1 X 1, 2 X 2, 3 X 3 and
 *          4 X 4 tiles.
 * https://oeis.org/A054856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1, a(2)=5, a(3)=13, a(4)=40; a(n) = 2*a(n-1) +
 *            3*a(n-2) - a(n-4) - a(n-5) for n ≥ 5
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of already computed terms */
static idx_t cache_size = 0;    /* number of elements stored in cache */

/* grow cache to hold entries 0 … needed-1, computing new values as required */
static void ensure(idx_t needed)
{
    if (needed < cache_size)
        return;                 /* already available */

    /* reallocate larger buffer */
    size_t newsize = (size_t)needed;     /* needed + 1 later, but needed is index, so +1 */
    if (newsize == 0) newsize = 1;       /* guard against zero size */

    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (tmp == NULL && newsize != 0)
        abort();                       /* allocation failure */
    cache = tmp;

    /* compute missing values */
    for (idx_t i = cache_size; i < newsize; ++i)
    {
        seq_t val;
        if (i == 0)          val = (seq_t)1;
        else if (i == 1)     val = (seq_t)1;
        else if (i == 2)     val = (seq_t)5;
        else if (i == 3)     val = (seq_t)13;
        else if (i == 4)     val = (seq_t)40;
        else
            val = 2 * cache[i-1] + 3 * cache[i-2] - cache[i-4] - cache[i-5];
        cache[i] = val;
    }
    cache_size = newsize;
}

seq_t a(idx_t n)
{
    /* we need indices 0 … n, so requested size is n+1 */
    if (n + 1 > cache_size)
        ensure(n + 1);
    return cache[n];
}
