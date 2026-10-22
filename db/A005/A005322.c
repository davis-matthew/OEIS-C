/*
 * A005322  Column of Motzkin triangle.
 * https://oeis.org/A005322
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)*(n+4) = (4*n+9)*a(n-1) - (n-1)*a(n-2) - 6*(n-1)*a(n-3),
 *            a(2) = 1, a(3) = 3, a(4) = 9. Each a(k) is split as q*(n+4) + r,
 *            so the full numerator is never formed
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 2) return 0;

    if (cache == NULL) {
        cache_size = 1024;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 3;
        cache[4] = 9;
        cache_len = 5;
    }

    if (n < cache_len) return cache[n];

    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    for (idx_t i = cache_len; i <= n; i++) {
        long long d = (long long)i + 4;
        long long c1 = 4 * (long long)i + 9;
        long long c2 = (long long)i - 1;
        long long c3 = 6 * ((long long)i - 1);
        seq_t sd = (seq_t)(idx_t)d;

        seq_t q1 = cache[i - 1] / sd;
        long long r1 = (long long)(cache[i - 1] % sd);
        seq_t q2 = cache[i - 2] / sd;
        long long r2 = (long long)(cache[i - 2] % sd);
        seq_t q3 = cache[i - 3] / sd;
        long long r3 = (long long)(cache[i - 3] % sd);

        long long S = c1 * r1 - c2 * r2 - c3 * r3; /* divisible by d */

        seq_t res = q1 * (seq_t)(idx_t)c1;
        if (S >= 0) res += (seq_t)(idx_t)(S / d);
        res -= q2 * (seq_t)(idx_t)c2;
        res -= q3 * (seq_t)(idx_t)c3;
        if (S < 0) res -= (seq_t)(idx_t)((-S) / d);
        cache[i] = res;
    }
    cache_len = n + 1;

    return cache[n];
}
