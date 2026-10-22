/*
 * A006630  From generalized Catalan numbers.
 * https://oeis.org/A006630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3*n+5)*(3*n+4)*(n+1)*a(n-1) / (2*n*(n+3)*(2*n+5)),
 *            a(0) = 1 (equivalently a(n) = 2*binomial(3*n+6, n)/(n+2))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed = 0; /* cache[0..computed-1] are valid */

static uint64_t gcd_u64(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_len = (n + 1 > 16) ? n + 1 : 16;
        cache = alloc_zero(cache_len);
        cache[0] = 1;
        computed = 1;
    } else if (n >= cache_len) {
        idx_t newlen = cache_len * 2;
        if (newlen < n + 1) newlen = n + 1;
        cache = grow_zero(cache, cache_len, newlen);
        cache_len = newlen;
    }

    while (computed <= n) {
        uint64_t i = (uint64_t)computed;
        uint64_t num = 3 * (3 * i + 5) * (3 * i + 4) * (i + 1);
        uint64_t den = 2 * i * (i + 3) * (2 * i + 5);
        uint64_t g = gcd_u64(num, den);
        num /= g;
        den /= g;
        /* den divides a(i-1) since num/den is reduced and the result is integral */
        seq_t v = cache[i - 1] / den;
        v = v * num;
        cache[i] = v;
        computed++;
    }

    return cache[n];
}
