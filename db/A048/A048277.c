/*
 * A048277  Number of (not necessarily distinct) nonsquarefree numbers among
 *          C(n,k), k=0..n.
 * https://oeis.org/A048277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑squarefree binomial coefficients in row n
 *            of Pascal's triangle. Computed by checking, for each k = 0..n,
 *            whether the exponent of some prime in C(n,k) is ≥2. Uses
 *            Legendre's formula for prime exponents in factorials
 * Range:     u8 n <= 260, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Sieve primes up to n */
    size_t array_n = (size_t)n;
    unsigned char *is_composite = (unsigned char *)calloc(array_n + 1, sizeof(unsigned char));
    if (!is_composite)
        return (seq_t)0;
    size_t primes_est = array_n / 2 + 1;
    uint64_t *primes = (uint64_t *)malloc(primes_est * sizeof(uint64_t));
    if (!primes) {
        free(is_composite);
        return (seq_t)0;
    }
    size_t pc = 0;
    for (size_t i = 2; i <= array_n; ++i) {
        if (!is_composite[i]) {
            primes[pc++] = (uint64_t)i;
            if (i * i <= array_n) {
                for (size_t j = i * i; j <= array_n; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);

    /* Precompute exponent of each prime in n! */
    uint64_t *exp_n = (uint64_t *)malloc(pc * sizeof(uint64_t));
    if (!exp_n) {
        free(primes);
        return (seq_t)0;
    }
    for (size_t pi = 0; pi < pc; ++pi) {
        uint64_t p = primes[pi];
        uint64_t sum = 0;
        uint64_t div = p;
        while (div <= (uint64_t)n) {
            sum += n / div;
            if (div > (uint64_t)n / p) break;
            div *= p;
        }
        exp_n[pi] = sum;
    }

    /* Count nonsquarefree binomial coefficients */
    seq_t count = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        bool nonsq = false;
        for (size_t pi = 0; pi < pc && !nonsq; ++pi) {
            uint64_t p = primes[pi];
            /* exponent of p in k! */
            uint64_t sum_k = 0;
            uint64_t div = p;
            while (div <= (uint64_t)k) {
                sum_k += k / div;
                if (div > (uint64_t)k / p) break;
                div *= p;
            }
            /* exponent of p in (n-k)! */
            uint64_t nk = n - k;
            uint64_t sum_nk = 0;
            div = p;
            while (div <= nk) {
                sum_nk += nk / div;
                if (div > nk / p) break;
                div *= p;
            }
            if (exp_n[pi] - sum_k - sum_nk >= 2)
                nonsq = true;
        }
        if (nonsq)
            count++;
    }

    free(primes);
    free(exp_n);
    return count;
}
