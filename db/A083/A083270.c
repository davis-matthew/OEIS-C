/*
 * A083270  Duplicate of A000879.
 * https://oeis.org/A083270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A000720(prime(n)^2), the number of primes < prime(n)^2;
 *            prime(n) from a sieve, pi(x) via Lucy_Hedgehog counting
 *            (duplicate of A000879)
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* pi(x) for x = r*r, Lucy_Hedgehog */
static uint64_t A000879_prime_pi_square(uint64_t r) {
    uint64_t x = r * r;
    uint64_t *lo = (uint64_t *)malloc((r + 2) * sizeof(uint64_t));
    uint64_t *hi = (uint64_t *)malloc((r + 2) * sizeof(uint64_t));
    for (uint64_t v = 0; v <= r; v++) lo[v] = v ? v - 1 : 0;
    for (uint64_t i = 1; i <= r; i++) hi[i] = x / i - 1;
    for (uint64_t p = 2; p <= r; p++) {
        if (lo[p] == lo[p - 1]) continue;
        uint64_t sp = lo[p - 1];
        uint64_t p2 = p * p;
        uint64_t lim = x / p2;
        if (lim > r) lim = r;
        for (uint64_t i = 1; i <= lim; i++) {
            uint64_t d = i * p;
            uint64_t val = (d <= r) ? hi[d] : lo[x / d];
            hi[i] -= val - sp;
        }
        for (uint64_t v = r; v >= p2; v--)
            lo[v] -= lo[v / p] - sp;
    }
    uint64_t res = hi[1];
    free(lo);
    free(hi);
    return res;
}

static uint64_t *A000879_primes = NULL;
static uint64_t A000879_nprimes = 0;
static uint64_t A000879_sieved_to = 0;

static void A000879_ensure_primes(uint64_t n) {
    uint64_t L = A000879_sieved_to ? A000879_sieved_to : 1024;
    while (A000879_nprimes < n) {
        L *= 2;
        unsigned char *s = (unsigned char *)malloc(L + 1);
        memset(s, 1, L + 1);
        s[0] = s[1] = 0;
        for (uint64_t i = 2; i * i <= L; i++)
            if (s[i])
                for (uint64_t j = i * i; j <= L; j += i) s[j] = 0;
        uint64_t cnt = 0;
        for (uint64_t i = 2; i <= L; i++) if (s[i]) cnt++;
        free(A000879_primes);
        A000879_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        A000879_nprimes = 0;
        for (uint64_t i = 2; i <= L; i++) if (s[i]) A000879_primes[A000879_nprimes++] = i;
        free(s);
        A000879_sieved_to = L;
    }
}

static seq_t A000879_a(idx_t n) {
    A000879_ensure_primes((uint64_t)n);
    uint64_t p = A000879_primes[n - 1];
    seq_t result = (seq_t)A000879_prime_pi_square(p);
    return result;
}

seq_t a(idx_t n) {
    return A000879_a(n);
}
