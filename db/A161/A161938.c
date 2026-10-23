/*
 * A161938  a(n) = ((3+sqrt(2))*(2+sqrt(2))^n + (3-sqrt(2))*(2-sqrt(2))^n)/2.
 * https://oeis.org/A161938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 2*a(n-2), a(0)=3, a(1)=8 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

static seq_t *cache = NULL;
static idx_t cap = 0;
static idx_t last_computed = (idx_t)-1; /* will be set to 1 after init */

static void init_cache(void)
{
    if (cache == NULL) {
        cache = seq_alloc_new_array(2);
        cap = 2;
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)8;
        last_computed = 1;
    }
}

static void ensure_capacity(idx_t n)
{
    if (cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (idx_t i = 0; i <= last_computed; ++i)
            new_cache[i] = cache[i];
        seq_free(cache);
        cache = new_cache;
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (cache == NULL)
        init_cache();

    /* grow capacity if necessary */
    ensure_capacity(n);

    /* compute missing terms */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)4 * cache[i - 1] - (seq_t)2 * cache[i - 2];
        }
        last_computed = n;
    }

    return cache[n];
}
