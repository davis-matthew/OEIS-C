/*
 * A180480  Numbers m such that m*m/k is an integer. m=(x_1 x_2 ... x_r) where
 *          x_i are digits of m, k = x_1^2 + x_2^2 + ... + x_r^2.
 * https://oeis.org/A180480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1515, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A180480: Numbers m such that (m*m)/k is an integer, 
   where k = sum of squares of the decimal digits of m. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true if m satisfies the defining property. */
static bool is_valid(seq_t m)
{
    seq_t temp = m;
    uint64_t k = 0;

    /* Compute k = sum of squares of decimal digits of m. */
    while (temp > (seq_t)0) {
        uint8_t d = (uint8_t)(temp % (seq_t)10);
        k += (uint64_t)d * (uint64_t)d;
        temp /= (seq_t)10;
    }
    /* m should be > 0, so k > 0 */
    if (k == 0u)
        return false;

    /* m*m divisible by k  <=>  (m % k)^2 % k == 0 */
    seq_t r = m % (seq_t)k;
    seq_t rr = r * r;
    return (rr % (seq_t)k) == (seq_t)0;
}

/* Return the n‑th element of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t len = 0;
    static seq_t current = 1;    /* Next candidate to test */

    if (n == 0u)
        return 0;                /* offset placeholder */

    /* Allocate or grow the cache to hold at least n elements. */
    if (len < n) {
        idx_t new_cap = (len > 0) ? len << 1 : 1024;
        if (new_cap < n)
            new_cap = n;
        if (!cache) {
            cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        }
        /* Fill the cache up to the requested index. */
        while (len < n) {
            while (!is_valid(current))
                ++current;
            cache[len++] = current;
            ++current;
        }
    }

    return cache[n - 1];
}
