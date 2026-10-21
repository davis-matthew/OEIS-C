/*
 * A083475  Consider the set of all the numbers n*k where 1 <= k <= n. Then
 *          a(n) = number of palindromic members of this set.
 * https://oeis.org/A083475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = #{k | 1 ≤ k ≤ n, is_palindrome(n·k)}
 * Range:     u8 n <= 15, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_pal_integ
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_pal_integ.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t last_computed = 0;

static void extend_cache(idx_t n) {
    if (n + 1 <= cache_size) return;
    size_t new_size = cache_size ? cache_size * 2 : 16;
    while (new_size <= (size_t) n) new_size *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard in case   */
    extend_cache(n);
    if (n <= last_computed) return cache[n];
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t count = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t prod = (seq_t)i * (seq_t)k;
            if (is_pal_integ(prod)) ++count;
        }
        cache[i] = count;
    }
    last_computed = n;
    return cache[n];
}
