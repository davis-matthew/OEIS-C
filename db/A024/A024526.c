/*
 * A024526  a(n) = Sum_{0 <= i < j <= n} (prime(j) - prime(i))^2, where
 *          prime(0) = 1.
 * https://oeis.org/A024526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*sum_{k=0..n} prime(k)^2 - (sum_{k=0..n} prime(k))^2
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where prime(0)=1 and prime(k) for k>=1 is the k-th prime (prime(1)=2). */
/* This implementation generates primes up to the n-th index each call. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Primality sieve up to computed limit. */
static void sieve_primes(idx_t need, uint64_t **out, size_t *cnt_out)
{
    /* Allocate maximum needed primes (need+1 because prime(0)=1). */
    /* Estimate upper bound for the nth prime (n >= 6): n*(ln n + ln ln n) + 3n */
    size_t limit;
    if (need < 6)
        limit = 15;
    else {
        double n = (double)need;
        limit = (size_t)(n * (log(n) + log(log(n))) + 3.0 * n);
    }

    /* Ensure limit is enough to get at least 'need' primes. */
    /* Simple check: keep increasing if not enough primes. */
    bool *is_composite = NULL;
    size_t max_limit = limit;
    while (1) {
        is_composite = (bool *)calloc(max_limit + 1, sizeof(bool));
        if (!is_composite) abort();
        size_t prime_count = 1;  /* prime(0)=1 */
        for (size_t i = 2; i <= max_limit; ++i) {
            if (!is_composite[i]) {
                ++prime_count;
                if (prime_count > need)
                    break;
                if (i * i <= max_limit)
                    for (size_t j = i * i; j <= max_limit; j += i)
                        is_composite[j] = true;
            }
        }
        if (prime_count > need)
            break;  /* enough primes */
        free(is_composite);
        max_limit = max_limit * 2;  /* double the limit and retry */
    }

    /* Build output array */
    size_t total = need + 1;  /* include prime(0)=1 */
    *out = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!*out) abort();
    (*out)[0] = 1;  /* prime(0) */
    size_t idx = 1;
    for (size_t i = 2; i <= max_limit && idx < total; ++i) {
        if (!is_composite[i]) {
            (*out)[idx++] = i;
        }
    }
    *cnt_out = total;  /* count includes prime(0) */
    free(is_composite);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Generate primes up to index n */
    uint64_t *primes = NULL;
    size_t count = 0;
    sieve_primes(n, &primes, &count);
    /* primes[0..n] are ready, primes[0]=1 */

    /* Compute sums */
    seq_t S = 0;
    seq_t Q = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t val = (seq_t)primes[k];   /* cast to seq_t to avoid overflow in loops */
        S += val;
        Q += val * val;
    }
    seq_t term1 = ((seq_t)n + 1) * Q;
    seq_t term2 = S * S;
    seq_t res = term1 - term2;

    free(primes);
    return res;
}
