/*
 * A181658  Row sums of A181657.
 * https://oeis.org/A181658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-2) - a(n-4) - a(n-6); Initial values: a(0)=1,
 *            a(1)=2, a(2)=3, a(3)=7, a(4)=8, a(5)=19
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  sz      = 0;
    static const seq_t base[6] = { (seq_t)1,
                                   (seq_t)2,
                                   (seq_t)3,
                                   (seq_t)7,
                                   (seq_t)8,
                                   (seq_t)19 };

    /* initialise cache if needed */
    if (sz < 6)
    {
        size_t new_sz = 6;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory, unlikely in tests */
        cache = tmp;
        sz = 6;
        for (size_t i = 0; i < 6; ++i)
            cache[i] = base[i];
    }

    if (n < sz)
        return cache[n];

    /* grow cache to fit up to index n */
    size_t new_sz = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!tmp) return (seq_t)0;
    cache = tmp;
    sz = new_sz;

    /* compute missing terms using the linear recurrence */
    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t val = ((seq_t)3 * cache[i-2]) - cache[i-4] - cache[i-6];
        cache[i] = val;
    }

    return cache[n];
}
