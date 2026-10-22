/*
 * A007272  Super ballot numbers: 60*(2n)!/(n!*(n+3)!).
 * https://oeis.org/A007272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+3)*a(n) = 2*(2*n-1)*a(n-1), a(0) = 10; i.e. a(n) =
 *            60*(2n)!/(n!*(n+3)!)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   g_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/g_u64.h"

static seq_t *cache = NULL;
static idx_t computed = 0; /* number of valid entries */
static idx_t cap = 0;

seq_t a(idx_t n) {
    if (n >= cap) {
        idx_t newcap = cap ? cap : 16;
        while (newcap <= n) newcap *= 2;
        seq_t *nc = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (nc == NULL) return 0;
        for (idx_t i = 0; i < computed; i++) nc[i] = cache[i];
        free(cache);
        cache = nc;
        cap = newcap;
    }
    if (computed == 0) {
        cache[0] = 10;
        computed = 1;
    }
    while (computed <= n) {
        idx_t i = computed;
        seq_t prev = cache[i - 1];
        uint64_t den = (uint64_t)i + 3;
        uint64_t mul = 2 * (2 * (uint64_t)i - 1);
        uint64_t r = (uint64_t)(prev % (seq_t)den);
        uint64_t g = g_u64(den, r); /* gcd(prev, den) */
        uint64_t d2 = den / g;
        uint64_t g2 = g_u64(mul, d2);
        mul /= g2;
        d2 /= g2;
        /* d2 should now be 1 since the result is an integer */
        cache[i] = (prev / (seq_t)g) * (seq_t)mul / (seq_t)d2;
        computed++;
    }
    return cache[n];
}
