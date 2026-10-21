/*
 * A229026  Expansion of 1/((1-x)*((1-5*x)^2)*(1-8*x)).
 * https://oeis.org/A229026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 19*a(n-1) - 123*a(n-2) + 305*a(n-3) - 200*a(n-4)
   Initial values: a(0)=1, a(1)=19, a(2)=238, a(3)=2490 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < cache_len) return cache[n];

    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) exit(1);   /* out of memory */
    cache = new_cache;

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)19;
        } else if (i == 2) {
            cache[i] = (seq_t)238;
        } else if (i == 3) {
            cache[i] = (seq_t)2490;
        } else {
            seq_t val = cache[i-1];
            val *= (seq_t)19;
            seq_t tmp = cache[i-2];
            tmp *= (seq_t)123;
            val -= tmp;
            tmp = cache[i-3];
            tmp *= (seq_t)305;
            val += tmp;
            tmp = cache[i-4];
            tmp *= (seq_t)200;
            val -= tmp;
            cache[i] = val;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
