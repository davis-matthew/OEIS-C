/*
 * A056324  Number of reversible string structures with n beads using a
 *          maximum of five different colors.
 * https://oeis.org/A056324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 34*a(n-2) - 16*a(n-3) + 247*a(n-4) -
 *            317*a(n-5) - 200*a(n-6) + 610*a(n-7) - 300*a(n-8) for n > 8
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=11,
 *            a(5)=32, a(6)=116, a(7)=455, a(8)=1993
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init[9] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)4,
                               (seq_t)11, (seq_t)32, (seq_t)116,
                               (seq_t)455, (seq_t)1993 };

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;

seq_t a(idx_t n)
{
    /* Ensure cache has space for index n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap >> 1 : 8;
        while (new_cap <= n) new_cap = new_cap << 1;
        seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            seq_t pos = (seq_t)11 * cache[i-1];
            pos += (seq_t)247 * cache[i-4];
            pos += (seq_t)610 * cache[i-7];

            seq_t neg = (seq_t)34  * cache[i-2];
            neg += (seq_t)16  * cache[i-3];
            neg += (seq_t)317 * cache[i-5];
            neg += (seq_t)200 * cache[i-6];
            neg += (seq_t)300 * cache[i-8];

            cache[i] = pos - neg;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
