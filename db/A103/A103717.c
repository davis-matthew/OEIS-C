/*
 * A103717  Denominators of sum_{k=1..n} 1/k^10 = Zeta(10,n).
 * https://oeis.org/A103717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of sum_{k=1..n} 1/k^10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   gcd_any, mul128, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_any.h"
#include "../utils/mul128.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition; safe guard for small n */
    if (n < 1) return 0;

    /* ---------- generate primes up to n ---------- */
    bool *is_prime = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    if (!is_prime) return 0;
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t k = p * p; k <= n; k += p) is_prime[k] = false;
        }
    }

    /* allocate array for primes (worst case n entries) */
    uint64_t *primes = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return 0;
    }
    idx_t prime_count = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime[i]) primes[prime_count++] = (uint64_t)i;
    }
    free(is_prime);

    /* ---------- compute L = lcm_{k=1..n} k^10 ---------- */
    seq_t L = 1;  /* start with multiplicative identity */
    for (idx_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        /* compute e = floor(log_p(n)) */
        idx_t e = 0;
        uint64_t pw = 1;
        while (pw <= n) {
            ++e;
            pw *= p;
        }
        --e; /* e is exponent of p in lcm(1..n) */
        if (e == 0) continue;
        idx_t exp10 = 10 * e; /* exponent of p in lcm(k^10) */
        seq_t pp = pow_seq((seq_t)p, exp10);
        L = mul128(L, pp);
    }
    free(primes);

    /* ---------- sum of reciprocals using common denominator L ---------- */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t denom_k10 = pow_seq((seq_t)k, 10);
        seq_t term = L / denom_k10;   /* exact division */
        sum += term;
    }

    /* ---------- reduce fraction sum/L ---------- */
    seq_t g = gcd_any(sum, L);
    seq_t denom = L / g;
    return denom;
}
