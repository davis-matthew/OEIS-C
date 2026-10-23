/*
 * A138635  a(n) =3*a(n-3)-3*a(n-6)+2*a(n-9).
 * https://oeis.org/A138635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = 3*a(n-3) - 3*a(n-6) + 2*a(n-9)
   Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=1,
   a(5)=1, a(6)=1, a(7)=2, a(8)=1 */

static seq_t *cache = NULL;      /* cache[0 .. cache_cap-1] */
static size_t cache_cap = 0;
static size_t cache_len = 0;     /* number of computed entries */

/* Ensure the cache can hold values up to index 'needed' (inclusive). */
static void ensure_cache_size(size_t needed)
{
    if (cache_cap > needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 9;
    if (new_cap <= needed)
        new_cap = needed + 1;       /* need at least needed+1 */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* n is unsigned */
    ensure_cache_size(idx);
    if (idx < cache_len)
        return cache[idx];

    /* Initial terms for indices 0..8 */
    static const seq_t init[9] = {0, 0, 1, 0, 1, 1, 1, 2, 1};

    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 9) {
            cache[i] = init[i];
        } else {
            seq_t t1 = cache[i - 3] * (seq_t)3;
            seq_t t2 = cache[i - 6] * (seq_t)3;
            seq_t t3 = cache[i - 9] * (seq_t)2;
            seq_t sum = t1 + t3;
            /* The recurrence guarantees non‑negative results; avoid
               underflow by checking the subtraction. */
            if (t2 > sum)
                cache[i] = 0;
            else
                cache[i] = sum - t2;
        }
    }
    cache_len = idx + 1;      /* update number of computed values */
    return cache[idx];
}
