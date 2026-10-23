/*
 * A182843  Number of composite integers greater than or equal to n whose
 *          proper divisors are all less than n.
 * https://oeis.org/A182843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + b(n-1) + c(n-1) where b(k) = 1 if k is prime
 *            else 0, c(k) = number of primes less than the smallest prime
 *            factor of k. a(1) = 0, a(2) = 0
 * Range:     u8 n <= 77, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    size_t sz = (size_t)(n + 1);

    /* is_prime[i] == 1 if i is prime */
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime)
        return (seq_t)0;
    memset(is_prime, 1, sz);
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;
    for (idx_t i = 2; i * i <= n; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = 0;

    /* smallest prime factor spf[i] */
    uint32_t *spf = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!spf) {
        free(is_prime);
        return (seq_t)0;
    }
    memset(spf, 0, sz * sizeof(uint32_t));
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i]) {
            spf[i] = (uint32_t)i;
            for (idx_t j = i; j <= n; j += i)
                if (!spf[j])
                    spf[j] = (uint32_t)i;
        }

    /* prefix prime count pi[i] = number of primes ≤ i */
    uint32_t *pi = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!pi) {
        free(is_prime);
        free(spf);
        return (seq_t)0;
    }
    pi[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        pi[i] = pi[i - 1] + (is_prime[i] ? 1u : 0u);

    /* compute a[1..n] */
    seq_t *a_arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!a_arr) {
        free(is_prime);
        free(spf);
        free(pi);
        return (seq_t)0;
    }
    a_arr[0] = (seq_t)0;
    if (n >= 1) a_arr[1] = (seq_t)0;
    if (n >= 2) a_arr[2] = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t prev = a_arr[i - 1];
        idx_t m = i - 1;
        seq_t b = is_prime[m] ? (seq_t)1 : (seq_t)0;
        uint32_t minp = spf[m];
        seq_t c = (seq_t)pi[minp - 1];
        a_arr[i] = prev + b + c;
    }

    seq_t result = a_arr[n];

    free(is_prime);
    free(spf);
    free(pi);
    free(a_arr);

    return result;
}
