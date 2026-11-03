/*
 * A024496  a(n) = (3/(8n-4))*C(4n,n).
 * https://oeis.org/A024496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3/(8n-4))*binom(4n,n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void ensure_primes_up_to(idx_t limit, idx_t **out, size_t *cnt)
{
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_max = 0;

    if (limit <= primes_max) {
        *out = primes;
        *cnt = primes_cnt;
        return;
    }

    // Allocate sieve array
    unsigned char *is_prime = (unsigned char *)calloc((size_t)limit + 1, sizeof(unsigned char));
    if (!is_prime) return;          /* allocation failed – unlikely in test harness */

    for (size_t i = 2; i <= (size_t)limit; ++i) is_prime[i] = 1;
    is_prime[0] = 0; is_prime[1] = 0;

    for (size_t i = 2; i * i <= (size_t)limit; ++i) if (is_prime[i]) {
        for (size_t j = i * i; j <= (size_t)limit; j += i) is_prime[j] = 0;
    }

    size_t new_cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) if (is_prime[i]) new_cnt++;

    idx_t *new_primes = (idx_t *)realloc(primes, new_cnt * sizeof(idx_t));
    if (!new_primes) {
        free(is_prime);
        return; /* allocation failed – unlikely */
    }
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) if (is_prime[i]) new_primes[idx++] = (idx_t)i;

    free(is_prime);
    primes = new_primes;
    primes_cnt = new_cnt;
    primes_max = limit;

    *out = primes;
    *cnt = primes_cnt;
}

static size_t exp_factorial(idx_t n, idx_t p)
{
    size_t cnt = 0;
    while (n) {
        n /= p;
        cnt += n;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* compute prime list up to 4n */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    ensure_primes_up_to(4 * n, &primes, &primes_cnt);

    seq_t result = (seq_t)1;
    idx_t denom = 8 * n - 4;

    for (size_t i = 0; i < primes_cnt && primes[i] <= 4 * n; ++i) {
        idx_t p = primes[i];
        size_t e_bin = exp_factorial(4 * n, p)
                     - exp_factorial(3 * n, p)
                     - exp_factorial(n, p);

        size_t e_den = 0;
        idx_t d = denom;
        while (d % p == 0) {
            d /= p;
            ++e_den;
        }

        size_t net;
        if (p == 3)
            net = e_bin - e_den + 1; /* multiply by the extra 3 */
        else
            net = e_bin - e_den;

        /* `net` is guaranteed >= 0 for the integer sequence */
        for (size_t k = 0; k < net; ++k) {
            result *= (seq_t)p;
        }
    }

    return result;
}
