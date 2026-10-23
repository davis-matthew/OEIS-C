/*
 * A025541  a(n) = (1/C(0,0) + 1/C(2,1) + ... + 1/C(2n,n))*L, where L =
 *          LCM{C(0,0), C(2,1),..., C(2n,n)}.
 * https://oeis.org/A025541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM{C(2k,k):0≤k≤n} * Σ_{k=0}^{n} 1/C(2k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below computes the sequence using prime exponents for the lcm
   and a recurrence for the central binomial coefficients.  It is compatible
   with all supported integer precisions. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

static int exp_fact(idx_t n, idx_t p) {
    int e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* simple sieve up to limit returning primes array and count */
static void sieve_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out) {
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    size_t sz = limit + 1;
    char *is_prime = (char *)malloc(sz);
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    idx_t *primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) primes[idx++] = i;
    free(is_prime);
    *primes_out = primes;
    *cnt_out = cnt;
}

seq_t a(idx_t n) {
    /* Handle n = 0 directly */
    if (n == 0) return 1;

    /* Compute C(2k,k) for k = 0..n */
    size_t len = (size_t)n + 1;
    seq_t *C = (seq_t *)malloc(len * sizeof(seq_t));
    if (!C) return 0;
    C[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t val = C[k - 1];
        val *= (2 * k);
        val *= (2 * k - 1);
        val /= k;
        val /= k;
        C[k] = val;
    }

    /* Find primes up to 2n for LCM calculation */
    idx_t prime_limit = 2 * n;
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(prime_limit, &primes, &prime_cnt);

    /* Compute max exponent of each prime in all C(2k,k) */
    int *prime_exp = (int *)malloc(prime_cnt * sizeof(int));
    for (size_t pi = 0; pi < prime_cnt; ++pi) prime_exp[pi] = 0;
    for (idx_t k = 0; k <= n; ++k) {
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            idx_t p = primes[pi];
            int e = exp_fact(2 * k, p) - 2 * exp_fact(k, p);
            if (e > prime_exp[pi]) prime_exp[pi] = e;
        }
    }

    /* Construct L = lcm of all C(2k,k) */
    seq_t L = 1;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        int e = prime_exp[pi];
        if (e > 0) {
            seq_t p_pow = pow_uint((uint64_t)primes[pi], (idx_t)e);
            L *= p_pow;
        }
    }

    /* Sum L / C(2k,k) for k=0..n */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = L / C[k];
        sum += term;
    }

    /* Clean up */
    free(C);
    free(primes);
    free(prime_exp);
    return sum;
}
