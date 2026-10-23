/*
 * A023833  Sum of exponents in prime-power factorization of C(4n,n+3).
 * https://oeis.org/A023833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω(C(4*n, n+3))
 * Range:     u8 n <= 751, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ω(x) = sum of exponents in the prime‐power factorisation of x */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Generate all primes ≤ limit.
   The caller must free the returned array. */
static void sieve_primes(idx_t limit, idx_t **out, size_t *cnt)
{
    if (limit < 2) {
        *out = NULL;
        *cnt = 0;
        return;
    }
    /* Simple sieve of Eratosthenes */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime) { *out = NULL; *cnt = 0; return; }
    memset(is_prime, 1, (size_t)(limit + 1));
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes first */
    size_t c = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++c;

    *out = (idx_t *)malloc(c * sizeof(idx_t));
    if (!*out) { free(is_prime); *cnt = 0; return; }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) (*out)[idx++] = i;

    *cnt = c;
    free(is_prime);
}

/* Compute v_p(n!) = sum_{k>=1} floor(n/p^k) without overflow */
static idx_t v_p_factorial(idx_t n, idx_t p)
{
    idx_t res = 0;
    while (n) {
        n /= p;
        res += n;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t four_n = 4 * n;
    idx_t k      = n + 3;
    idx_t m      = four_n - k;   /* 3*n - 3 */

    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(four_n, &primes, &prime_cnt);

    seq_t sum = 0;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t e4 = v_p_factorial(four_n, p);
        idx_t ek = v_p_factorial(k, p);
        idx_t em = v_p_factorial(m, p);
        idx_t diff = e4;
        if (diff > ek) diff -= ek; else diff = 0;
        if (diff > em) diff -= em; else diff = 0;
        if (diff) sum += (seq_t)diff;
    }

    free(primes);
    return sum;
}
