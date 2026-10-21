/*
 * A066203  a(1) = 2; a(2) = 1; for n > 2, a(n) = a(n-1)-(n-1) if this is
 *          positive and has not already appeared in the sequence, otherwise
 *          a(n) = a(n-1)+(n-1).
 * https://oeis.org/A066203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1) = 2, a(2) = 1; for n>2,
   a(n) = a(n-1)-(n-1) if this is positive and has not appeared
   before, otherwise a(n) = a(n-1)+(n-1) */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *cache   = NULL;   /* array holding generated terms   */
static size_t cache_cap = 0;     /* current capacity of the array   */
static size_t cache_len = 0;     /* number of terms stored so far  */

/* ensure the cache can hold at least 'needed' elements */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 4 : cache_cap;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache   = tmp;
    cache_cap = new_cap;
}

/* build the sequence up to index 'up' (inclusive) */
static void build_until(idx_t up)
{
    if (up <= cache_len)
        return;

    for (idx_t n = cache_len + 1; n <= up; ++n) {
        seq_t value;

        if (n == 1) {
            value = 2;
        } else if (n == 2) {
            value = 1;
        } else {
            seq_t prev   = cache[n - 2];          /* a(n-1)   */
            idx_t step   = n - 1;
            seq_t default_add = prev + (seq_t)step; /* a(n-1)+(n-1) */

            seq_t candidate = default_add;      /* default candidate */
            bool use_subtract = false;

            if (prev > (seq_t)step) {           /* subtraction would be positive */
                candidate = prev - (seq_t)step; /* a(n-1)-(n-1)   */
                bool seen = false;
                for (size_t j = 0; j < n - 1; ++j) {
                    if (cache[j] == candidate) {
                        seen = true;
                        break;
                    }
                }
                if (!seen)
                    use_subtract = true;
            }

            value = use_subtract ? candidate : default_add;
        }

        ensure_cache(cache_len + 1);
        cache[cache_len++] = value;
    }
}

seq_t a(idx_t n)
{
    build_until(n);
    return cache[n - 1];
}
