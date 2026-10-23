/*
 * A190096  Number of rhombuses on a (n+1)X8 grid.
 * https://oeis.org/A190096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n>51; initial values for
 *            1 ≤ n ≤ 51 are computed by brute force. a(1)=7, a(2)=32,
 *            a(3)=82, a(4)=164, a(5)=273, a(6)=412, a(7)=564, a(8)=736,
 *            a(9)=918, a(10)=1112, a(11)=1306, a(12)=1520, a(13)=1734,
 *            a(14)=1960, a(15)=2198, a(16)=2448, a(17)=2698, a(18)=2964...
 * Range:     u8 n <= 2, u16 n <= 123, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   brute_compute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/brute_compute.h"

/* Cache for computed values.
   cache[i] holds a(i+1) for i ≥ 0. */
static seq_t *cache           = NULL;
static size_t cache_cap       = 0;   /* current capacity of cache */
static idx_t  max_computed    = 0;   /* highest n that has been computed (inclusive) */

/* Ensure the cache is large enough to store values up to n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = (size_t)n;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) return;            /* allocation failed – caller will return 0 */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset */

    ensure_cache(n);
    if (!cache) return 0;   /* realloc failed */

    /* Compute missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 51) {
            val = brute_compute(i);
        } else {
            /* recurrence: a(i) = 2*a(i-1) - 2*a(i-3) + a(i-4) */
            val = 2 * cache[(size_t)(i - 2)]   /* a(i-1) */
                  - 2 * cache[(size_t)(i - 4)]   /* a(i-3) */
                  + cache[(size_t)(i - 5)];       /* a(i-4) */
        }
        cache[(size_t)(i - 1)] = val;
    }
    max_computed = n;
    return cache[(size_t)(n - 1)];
}
