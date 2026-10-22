/*
 * A004594  Expansion of e in base 3.
 * https://oeis.org/A004594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(1) = 2 = floor(e); for n >= 2, a(n) = floor(3^(n-1)*e) mod 3,
 *            computed by a spigot: e - 2 = Sum_{k>=2} 1/k! is 0.111... in the
 *            factorial base, and each base-3 digit comes from multiplying
 *            that mixed-radix fraction by 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static unsigned char *cache = NULL;
static long long cache_len = 0; /* number of fractional base-3 digits cached */

static void compute_digits(long long m) {
    /* Choose N with ln(N!) > (m + 40) * ln 3, so the truncation error is negligible. */
    double target = (double)(m + 40) * log(3.0);
    double acc = 0.0;
    long long N = 2;
    while (acc <= target) { N++; acc += log((double)N); }
    N += 10;

    int *d = (int *)malloc((size_t)(N + 1) * sizeof(int));
    for (long long k = 0; k <= N; k++) d[k] = 1;

    unsigned char *out = (unsigned char *)malloc((size_t)m + 1);
    for (long long i = 0; i < m; i++) {
        int carry = 0;
        for (long long k = N; k >= 2; k--) {
            int x = d[k] * 3 + carry;
            d[k] = x % (int)k;
            carry = x / (int)k;
        }
        out[i] = (unsigned char)carry;
    }
    free(d);
    free(cache);
    cache = out;
    cache_len = m;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)2;
    long long need = (long long)(n - 1);
    if (need > cache_len) {
        long long m = need * 2;
        if (m < 256) m = 256;
        compute_digits(m);
    }
    return (seq_t)cache[need - 1];
}
