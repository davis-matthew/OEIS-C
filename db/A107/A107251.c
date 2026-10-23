/*
 * A107251  Supercatalan numbers SF(2n)/(SF(n)*SF(n+1)) where SF is the
 *          superfactorial function A000178.
 * https://oeis.org/A107251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = SF(2n) / (SF(n) * SF(n+1)) where SF(m) = ∏_{k=1}^{m} k!
 *            (superfactorial). This implementation uses exponentiation by
 *            prime factorization of the superfactorials; no floating point
 *            arithmetic is used
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   exp_sf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires interface of seq_t and idx_t as provided by the build. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/exp_sf.h"

/* Prime table (max prime needed for n <= 10000 is 20000). */
static int primes[3000];
static int prime_count = 0;
static uint64_t prime_limit = 0;

/* Simple sieve to fill primes up to N (at least once). */
static void ensure_primes(uint64_t N) {
    if (prime_limit >= N) return;
    int max_n = (int)N;
    char *is_prime = (char*)malloc((max_n + 1) * sizeof(char));
    if (!is_prime) return; /* silently fail if memory allocation fails */
    for (int i = 2; i <= max_n; ++i) is_prime[i] = 1;
    for (int i = 2; i * i <= max_n; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= max_n; j += i)
                is_prime[j] = 0;
    prime_count = 0;
    for (int i = 2; i <= max_n; ++i)
        if (is_prime[i]) primes[prime_count++] = i;
    free(is_prime);
    prime_limit = N;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    uint64_t nn = (uint64_t)n;
    uint64_t max_p = 2 * nn;
    ensure_primes(max_p);

    seq_t result = (seq_t)1;

    for (int i = 0; i < prime_count; ++i) {
        int p = primes[i];
        if (p > (int)max_p) break;

        uint64_t exp = exp_sf(2 * nn, p) - exp_sf(nn + 1, p) - exp_sf(nn, p);
        if (exp == 0) continue;

        seq_t power = (seq_t)1;
        seq_t base = (seq_t)p;
        uint64_t e = exp;
        seq_t cur = base;
        while (e) {
            if (e & 1ULL) power *= cur;
            e >>= 1;
            if (e) cur *= cur;
        }
        result *= power;
    }
    return result;
}
