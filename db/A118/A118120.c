/*
 * A118120  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+17)^2 = y^2.
 * https://oeis.org/A118120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=7, a(2)=28, a(3)=51, a(4)=88, a(5)=207,
 *            a(n)=6*a(n-3)-a(n-6)+34 for n>5 (OEIS sequence A118120)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 33, u64 n <= 71, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* memoised recursive computation of A118120 */
static seq_t *cache = NULL;
static idx_t cached_len = 0;
static idx_t cached_cap = 0;

/* helper: grow the cached array to at least new_cap elements */
static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cached_cap) return;
    size_t new_bytes = (size_t)new_cap * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, new_bytes);
    if (!tmp) abort();                /* insufficient memory */
    cache = tmp;
    cached_cap = new_cap;
}

/* initialise the cache with the first six terms */
static void init_cache(void)
{
    static const seq_t first[6] = {0, 7, 28, 51, 88, 207};
    grow_cache(6);
    for (idx_t i = 0; i < 6; ++i) cache[i] = first[i];
    cached_len = 6;
}

/* sequence function required by the build */
seq_t a(idx_t n)
{
    if (!cache) init_cache();

    /* compute terms up to n */
    if (n >= cached_cap) grow_cache(n + 1);
    if (n >= cached_len) {
        for (idx_t i = cached_len; i <= n; ++i) {
            seq_t val;
            if (i <= 5) {
                /* these values were already initialised */
                val = cache[i];
            } else {
                val = 6 * cache[i - 3] - cache[i - 6] + (seq_t)34;
            }
            cache[i] = val;
        }
        cached_len = n + 1;
    }
    return cache[n];
}
