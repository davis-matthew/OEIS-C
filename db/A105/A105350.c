/*
 * A105350  Largest squared factorial dividing n!.
 * https://oeis.org/A105350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m!)^2, where m = max{k ≥ 0 : (k!)^2 divides n!}
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Legendre exponent of prime p in n! */
static inline uint32_t legendre_exp(idx_t n, uint32_t p)
{
    uint32_t exp = 0;
    uint64_t t = n;
    while (t) {
        t /= p;
        exp += (uint32_t)t;
    }
    return exp;
}

seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n < 2)
        return (seq_t)1;          /* 0!^2 = 1, 1!^2 = 1 */

    /* ---- 1. generate list of primes ≤ n ---- */
    idx_t N = n;
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(N + 1));
    if (!is_prime)
        return (seq_t)1;
    for (idx_t i = 0; i <= N; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= N; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= N; j += i)
                is_prime[j] = 0;

    /* Count primes */
    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= N; ++i)
        if (is_prime[i])
            ++prime_cnt;

    /* Allocate arrays */
    uint32_t *primes = (uint32_t *)malloc((size_t)prime_cnt * sizeof(uint32_t));
    uint32_t *v_n    = (uint32_t *)malloc((size_t)prime_cnt * sizeof(uint32_t));
    if (!primes || !v_n) {
        free(is_prime);
        free(primes); free(v_n);
        return (seq_t)1;
    }

    /* Fill primes and exponents of n! */
    idx_t pos = 0;
    for (idx_t i = 2; i <= N; ++i)
        if (is_prime[i]) {
            primes[pos] = (uint32_t)i;
            v_n[pos] = legendre_exp(n, (uint32_t)i);
            ++pos;
        }
    free(is_prime);

    /* ---- 2. binary search for largest m ---- */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* upper mid */
        bool ok = true;
        for (idx_t k = 0; k < prime_cnt && primes[k] <= mid; ++k) {
            uint32_t vp_mid = legendre_exp(mid, primes[k]);
            if ((uint64_t)v_n[k] < (uint64_t)vp_mid * 2ull) {
                ok = false;
                break;
            }
        }
        if (ok)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t m = low;

    /* ---- 3. compute (m!)^2 ---- */
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k < prime_cnt && primes[k] <= m; ++k) {
        uint32_t vp_m = legendre_exp(m, primes[k]);
        uint32_t exp  = vp_m * 2u;
        uint32_t p    = primes[k];
        for (uint32_t e = 0; e < exp; ++e)
            result *= (seq_t)p;
    }

    free(primes);
    free(v_n);
    return result;
}
