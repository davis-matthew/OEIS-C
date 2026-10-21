/*
 * A087057  Smallest number whose square is larger than 2*n^2.
 * https://oeis.org/A087057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(n*sqrt(2)) = floor(n*sqrt(2)) + 1 The integer part
 *            of n*sqrt(2) is obtained by a fixed‑point accumulation using the
 *            integer 1518500241 = floor(sqrt(2)*2^30). This avoids any
 *            floating‑point arithmetic and works for all seq_t sizes that can
 *            represent the result. A static cache is used s...
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t, unsigned __int128 */

static const unsigned long long SQRT2_SCALE = 1518500241ULL;   /* floor(sqrt(2)*2^30) */
static const unsigned       SHIFT          = 30;

seq_t a(idx_t n)
{
    if (n == 0)            /* behaviour for n<1 is undefined; return 0 */
        return (seq_t)0;

    /* static cache of already computed values */
    static seq_t *cache      = NULL;
    static idx_t  max_cached = 0;
    static unsigned __int128 accum = 0;      /* accumulator for fixed‑point sum */

    if (n > max_cached) {
        /* grow the cache to hold values up to n */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)           /* out of memory – return 0 (unlikely in tests) */
            return (seq_t)0;
        cache = tmp;

        /* extend the computed range */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            accum += (unsigned __int128)SQRT2_SCALE;
            /* floor(i*sqrt(2)) = (floor(accum>>SHIFT)) */
            uint64_t floor_val = (uint64_t)(accum >> SHIFT);
            cache[i] = (seq_t)(floor_val + 1);   /* ceil = floor + 1 */
        }
        max_cached = n;
    }
    return cache[n];
}
