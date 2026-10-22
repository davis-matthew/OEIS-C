/*
 * A002070  Coefficient of x^p (p = n-th prime) in x * Product_{k>=1}
 *          (1-x^k)^2*(1-x^11k)^2.
 * https://oeis.org/A002070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = p - #{(x,y) mod p : y^2 + y == x^3 - x^2 (mod p)}, p =
 *            prime(n); this is the a_p trace of the elliptic curve 11a3,
 *            whose L-series is x*Product_{k>=1} (1-x^k)^2*(1-x^11k)^2. Primes
 *            come from a sieve
 * Range:     i8 n <= 638, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a2070_primes = NULL;
static uint64_t a2070_np = 0;

static uint64_t a2070_nth_prime(idx_t n) {
    uint64_t lim = 1024;
    while (a2070_np < n) {
        lim *= 2;
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        uint64_t cnt = 0, i, j;
        for (i = 2; i <= lim; i++) {
            if (!comp[i]) {
                cnt++;
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
            }
        }
        if (cnt >= n) {
            free(a2070_primes);
            a2070_primes = (uint64_t *)malloc((size_t)cnt * sizeof(uint64_t));
            a2070_np = 0;
            for (i = 2; i <= lim; i++)
                if (!comp[i]) a2070_primes[a2070_np++] = i;
        }
        free(comp);
    }
    return a2070_primes[n - 1];
}

seq_t a(idx_t n) {
    uint64_t p = a2070_nth_prime(n);
    uint64_t *cnt = (uint64_t *)calloc((size_t)p, sizeof(uint64_t));
    uint64_t y, x, N = 0;
    for (y = 0; y < p; y++) cnt[(y * y + y) % p]++;
    for (x = 0; x < p; x++) {
        uint64_t x2 = x * x % p;
        uint64_t x3 = x2 * x % p;
        uint64_t v = (x3 + p - x2) % p;
        N += cnt[v];
    }
    free(cnt);
    long long r = (long long)p - (long long)N;
    seq_t res = (seq_t)r;
    return res;
}
