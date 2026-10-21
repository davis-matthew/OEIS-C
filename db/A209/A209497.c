/*
 * A209497  Number of n X 2 0..4 arrays with every 2 X 2 subblock containing
 *          exactly one value repeat, and new values 0..4 introduced in row
 *          major order.
 * https://oeis.org/A209497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=6, a(3)=57, a(4)=690, a(5)=9393 a(n) = 16*a(n-1) -
 *            10*a(n-2) - 168*a(n-3) - 72*a(n-4) for n>5 Recurrence proven
 *            empirically, used to compute every term
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   grow_seq, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mul_small.h"   /* mul_small(seq_t, unsigned) */
#include "../utils/grow_seq.h"    /* grow_seq(seq_t**,size_t*,size_t) */

static seq_t *cache = NULL;        /* 1‑based storage of computed values   */
static size_t cache_cap = 0;       /* capacity of cache array               */
static idx_t cache_max = 0;        /* largest index that has been filled    */

/* Ensure that cache[n] is valid, computing all missing entries. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    if (n + 1 > cache_cap)         /* need more space (index 0 unused)    */
        grow_seq(&cache, &cache_cap, n + 1);

    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[i] = 2;
        else if (i == 2)
            cache[i] = 6;
        else if (i == 3)
            cache[i] = 57;
        else if (i == 4)
            cache[i] = 690;
        else if (i == 5)
            cache[i] = 9393;
        else
        {
            seq_t t1 = mul_small(cache[i-1], 16);
            seq_t t2 = mul_small(cache[i-2], 10);
            seq_t t3 = mul_small(cache[i-3], 168);
            seq_t t4 = mul_small(cache[i-4], 72);
            seq_t val = t1;
            val -= t2;
            val -= t3;
            val -= t4;
            cache[i] = val;
        }
    }
    cache_max = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)            /* below offset: return 0, undefined behaviour otherwise */
        return 0;
    ensure_cache(n);
    return cache[n];
}
