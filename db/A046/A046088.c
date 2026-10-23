/*
 * A046088  Row sums of convolution triangle A030527.
 * https://oeis.org/A046088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 26*a(n-1) - 260*a(n-2) + 1300*a(n-3) - 3250*a(n-4) +
 *            3250*a(n-5) * for n ≥ 6. Initial terms a(1)=1, a(2)=16,
 *            a(3)=206, a(4)=2371, a(5)=25761 * are obtained from the
 *            generating function of the sequence
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_mul.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1);

    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        if (i <= 5)              /* initial values */
        {
            static const seq_t init[] = { (seq_t)0, (seq_t)1, (seq_t)16,
                                          (seq_t)206, (seq_t)2371, (seq_t)25761 };
            new_cache[i] = init[i];
        }
        else                     /* recurrence for n ≥ 6 */
        {
            seq_t val = seq_mul((seq_t)26, new_cache[i - 1]);
            val = val - seq_mul((seq_t)260, new_cache[i - 2]);
            val = val + seq_mul((seq_t)1300, new_cache[i - 3]);
            val = val - seq_mul((seq_t)3250, new_cache[i - 4]);
            val = val + seq_mul((seq_t)3250, new_cache[i - 5]);

            new_cache[i] = val;
        }
    }

    cache = new_cache;
    cache_len = n;
}

/* External linkage required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) is undefined; return 0 for safety */
    ensure_cache(n);
    return cache[n];
}
