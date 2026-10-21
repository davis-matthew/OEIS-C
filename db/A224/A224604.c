/*
 * A224604  Number of (n+2) X 8 0..2 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) for n>8, with initial values: a(1)=1045, a(2)=894,
 *            a(3)=1086, a(4)=1247, a(5)=1609, a(6)=2092, a(7)=2656, a(8)=3497
 * Range:     u8 none, u16 n <= 16, u32 n <= 45, u64 n <= 103, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Initial seven terms plus a dummy entry at index 0 */
static const seq_t base_values[9] = {0,
    1045, 894, 1086, 1247, 1609, 2092, 2656, 3497};

static void ensure_cache(idx_t n)
{
    if (cache_size <= n)
    {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();  /* panic on allocation failure */
        cache = new_cache;
        for (idx_t i = old_size; i <= n; ++i)
        {
            if (i <= 8)
            {
                cache[i] = base_values[i];
            }
            else
            {
                /* compute a(i) using the recurrence, ensuring no
                   intermediate underflow for unsigned types */
                seq_t pos = (seq_t)4 * cache[i - 1]
                          + (seq_t)5 * cache[i - 3]
                          + (seq_t)3 * cache[i - 5];
                seq_t neg = (seq_t)6 * cache[i - 2]
                          + (seq_t)4 * cache[i - 4]
                          + cache[i - 6];
                cache[i] = pos - neg;
            }
        }
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not part of the sequence, safe guard */
    ensure_cache(n);
    return cache[n];
}
