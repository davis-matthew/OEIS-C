/*
 * A026031  a(n) = number of (s(0), s(1), ..., s(2n)) such that s(i) is a
 *          nonnegative integer and |s(i) - s(i-1)| = 1 for i = 1,2,...,n,
 *          s(0) = 3, s(2n) = 7. Also a(n) = T(2n,n-2), where T is defined in
 *          A026022.
 * https://oeis.org/A026031
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n, n-2) - C(2n, n-6)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- prime cache utilities ---------- */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_cache_limit) return;

    size_t size = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(size);
    if (!is_prime) exit(1);

    for (size_t i = 0; i < size; ++i) is_prime[i] = true;
    if (size > 0) is_prime[0] = false;
    if (size > 1) is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) if (is_prime[i]) ++cnt;

    uint64_t *new_primes = (uint64_t *)realloc(prime_cache, cnt * sizeof(uint64_t));
    if (!new_primes) exit(1);
    prime_cache = new_primes;

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) prime_cache[idx++] = (uint64_t)i;
    }
    prime_cache_cnt = cnt;
    prime_cache_limit = limit;

    free(is_prime);
}

/* ---------- binomial via prime factorisation ---------- */
static seq_t binomial_prime(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;

    ensure_primes_up_to(n);

    seq_t result = (seq_t)1;
    for (size_t pi = 0; pi < prime_cache_cnt; ++pi) {
        uint64_t p = prime_cache[pi];
        if (p > n) break;

        idx_t exp = 0;

        /* exponent from n! */
        idx_t power = p;
        while (power <= n) {
            exp += n / power;
            if (power > n / p) break;   /* prevent overflow */
            power *= p;
        }

        /* subtract exponent from k! */
        power = p;
        while (power <= k) {
            exp -= k / power;
            if (power > k / p) break;
            power *= p;
        }

        /* subtract exponent from (n-k)! */
        idx_t nk = n - k;
        power = p;
        while (power <= nk) {
            exp -= nk / power;
            if (power > nk / p) break;
            power *= p;
        }

        /* multiply result by p^exp */
        for (idx_t i = 0; i < exp; ++i) {
            result = result * (seq_t)p;
        }
    }
    return result;
}

/* ---------- main sequence routine ---------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)2) return (seq_t)0;

    seq_t term1 = binomial_prime((idx_t)2 * n, n - (idx_t)2);
    seq_t term2 = ((n >= (idx_t)6) ?
                   binomial_prime((idx_t)2 * n, n - (idx_t)6) :
                   (seq_t)0);

    return term1 - term2;
}
