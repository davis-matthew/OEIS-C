/*
 * A126637  Difference x-y of generator pairs (x,y) {x and y coprime and not
 *          both odd, x>y} of primitive Pythagorean triangles, sorted on
 *          values x+y (A126611), then on x-y.
 * https://oeis.org/A126637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = difference (x - y) of primitive Pythagorean generator
 *            pairs (x, y) sorted first by the odd sum x + y and then by the
 *            difference x - y
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Euclidean algorithm for uint64_t. */
static inline uint64_t _gcd_u64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, not used per OEIS offset */

    /* Static cache of the sequence values. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap = 0;
    static uint64_t next_sum = 3;           /* next odd sum to process (x + y) */

    /* Extend cache until it contains at least n elements. */
    while (cache_size < n) {
        uint64_t s = next_sum;
        uint64_t half = (s - 1) / 2;          /* number of k to test */

        for (uint64_t kk = half; ; --kk) {
            if (_gcd_u64(kk, s) == 1) {
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();         /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)(s - 2ULL * kk);
            }
            if (kk == 1) break;               /* avoid underflow */
        }

        next_sum += 2;                       /* move to next odd sum */
    }

    return cache[n - 1];
}
