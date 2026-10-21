/*
 * A086666  a(n) = sigma_2(n) - sigma_1(n).
 * https://oeis.org/A086666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_2(n) - sigma_1(n) (sum of squares of divisors minus
 *            sum of divisors)
 * Range:     u8 n <= 15, u16 n <= 215, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_mul.h"

static int *prime_list = NULL;
static size_t prime_cnt = 0;

/* Build a list of primes once, up to a limit that comfortably exceeds sqrt(n)
   for n up to about 10^10 (sufficient for the required range). */
static void init_primes(void) {
    const size_t LIMIT = 200000;          /* sqrt(10^10) ≈ 100000; we take 200000 for safety */
    bool *sieve = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!sieve) return;
    memset(sieve, 0, (LIMIT + 1) * sizeof(bool));

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;
        }

    /* Count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) ++prime_cnt;

    prime_list = (int *)malloc(prime_cnt * sizeof(int));
    if (!prime_list) {
        free(sieve);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) prime_list[idx++] = (int)i;

    free(sieve);
}

/* Factor n and compute σ₁(n) and σ₂(n).  */
static void compute_sigmas(idx_t n, seq_t *out_sigma1, seq_t *out_sigma2) {
    if (!prime_list) init_primes();

    idx_t m = n;
    seq_t sigma1 = 1;
    seq_t sigma2 = 1;

    for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= m; ++i) {
        int p = prime_list[i];
        if (m % p == 0) {
            seq_t pow = 1;          /* current power of p: p^k  (k starts at 0) */
            seq_t sum1 = 1;         /* σ₁ contribution from this prime   */
            seq_t sum2 = 1;         /* σ₂ contribution from this prime   */

            while (m % p == 0) {
                m /= p;
                pow = seq_mul(pow, (seq_t)p);   /* multiply by p → p^(k+1) */
                sum1 += pow;                    /* add p^(k+1) to σ₁ sum   */
                sum2 += seq_mul(pow, pow);      /* add p^(2(k+1)) to σ₂ sum   */
            }

            sigma1 = seq_mul(sigma1, sum1);
            sigma2 = seq_mul(sigma2, sum2);
        }
    }

    /* Remaining prime factor, if any (exponent 1). */
    if (m > 1) {
        seq_t sum1 = 1 + (seq_t)m;
        seq_t sum2 = 1 + seq_mul((seq_t)m, (seq_t)m);
        sigma1 = seq_mul(sigma1, sum1);
        sigma2 = seq_mul(sigma2, sum2);
    }

    *out_sigma1 = sigma1;
    *out_sigma2 = sigma2;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t sigma1, sigma2;
    compute_sigmas(n, &sigma1, &sigma2);
    return sigma2 - sigma1;
}
