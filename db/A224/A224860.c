/*
 * A224860  Numbers n such that 90*n + 59 and 90*n + 61 are twin prime.
 * https://oeis.org/A224860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer n such that 90*n+59 and 90*n+61 are
 *            primes
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    if (n == 0)
        return 0;           /* offset is 1; n==0 is outside the sequence */

    if ((idx_t)cache_size >= n)
        return cache[n - 1];

    seq_t start = 0;
    if (cache_size > 0)
        start = cache[cache_size - 1] + 1;

    while ((idx_t)cache_size < n) {
        seq_t x = start++;
        idx_t p1 = 90u * (idx_t)x + 59;
        idx_t p2 = 90u * (idx_t)x + 61;
        if (is_prime(p1) && is_prime(p2)) {
            ensure_capacity(cache_size + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = x;
        }
    }
    return cache[n - 1];
}
