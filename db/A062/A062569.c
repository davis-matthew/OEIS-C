/*
 * A062569  a(n) = sigma(n!).
 * https://oeis.org/A062569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = σ(n!) = product_{p ≤ n} (p^{e+1}−1)/(p−1), where e =
 *            ∑_{k≥1} ⌊n / p^k⌋ is the exponent of p in n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* Ensure that prime_list contains all primes ≤ n. */
static void ensure_primes(idx_t n) {
    if (prime_cap >= (size_t)n) return;

    size_t new_cap = prime_cap ? prime_cap * 2 : 32;
    while (new_cap < (size_t)n) new_cap <<= 1;

    bool *is_prime = (bool *)malloc(new_cap + 1);
    for (size_t i = 0; i <= new_cap; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= new_cap; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= new_cap; j += i) is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= new_cap; ++i) if (is_prime[i]) ++cnt;
    uint32_t *new_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));

    size_t idx = 0;
    for (size_t i = 2; i <= new_cap; ++i) if (is_prime[i]) new_primes[idx++] = (uint32_t)i;

    free(is_prime);
    free(prime_list);
    prime_list = new_primes;
    prime_cnt = cnt;
    prime_cap = new_cap;
}

/* Fast exponentiation for seq_t values. */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n <= 1) return 1;

    ensure_primes(n);

    seq_t sigma = 1;

    for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        idx_t denom = p;
        idx_t e = 0;
        while (denom <= n) {
            e += n / denom;
            if (denom > n / p) break;   /* prevent overflow of denom */
            denom *= p;
        }

        seq_t base = (seq_t)p;
        seq_t pw = pow_seq_t(base, e + 1);                 /* p^(e+1) */
        seq_t term = (pw - 1) / (p - 1);                   /* (p^(e+1)-1)/(p-1) */
        sigma *= term;
    }

    return sigma;
}
