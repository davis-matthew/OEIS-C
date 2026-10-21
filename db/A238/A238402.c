/*
 * A238402  Number of ways to write n = p^2 + q - pi(q) with p prime and q
 *          among 1, ..., n, where pi(.) is given by A000720.
 * https://oeis.org/A238402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ p prime, 1 ≤ q ≤ n : n = p^2 + q - pi(q) }| where
 *            pi(q) is the prime counting function (A000720)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* integer floor of square root (works for 64‑bit indices) */
static inline idx_t floor_isqrt(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;            /* safe because mid ≤ sqrt(n) */
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */

    /* --- Sieve of Eratosthenes up to n --- */
    bool *is_prime = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    if (!is_prime) return 0;
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    /* --- Prime counting array pi[0..n] --- */
    idx_t *pi = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!pi) { free(is_prime); return 0; }
    pi[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        pi[i] = pi[i - 1] + (is_prime[i] ? 1 : 0);
    idx_t pi_n = pi[n];

    /* --- Count occurrences of d = q - pi(q) for 1 ≤ q ≤ n --- */
    seq_t *count_f = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!count_f) { free(is_prime); free(pi); return 0; }
    for (idx_t i = 0; i <= n; ++i) count_f[i] = 0;
    for (idx_t q = 1; q <= n; ++q) {
        idx_t d = q - pi[q];
        ++count_f[d];
    }

    /* --- Sum contributions of primes p with p^2 ≤ n --- */
    seq_t result = 0;
    idx_t sqrt_n = floor_isqrt(n);
    for (idx_t p = 2; p <= sqrt_n; ++p) {
        if (!is_prime[p]) continue;
        idx_t d = n - p * p;
        if (d >= 1 && d <= n - pi_n)
            result += count_f[d];
    }

    free(is_prime);
    free(pi);
    free(count_f);
    return result;
}
