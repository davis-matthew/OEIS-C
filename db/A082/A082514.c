/*
 * A082514  a(n) = pi(n) + tau(n).
 * https://oeis.org/A082514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = π(n) + τ(n) (π(n) = number of primes ≤ n, τ(n) = number
 *            of divisors of n)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static state: primes list and a simple incremental sieve */
static idx_t *primes = NULL;          /* prime list */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static unsigned char *prime_mark = NULL;  /* 0 = prime (default), 1 = composite */
static idx_t prime_mark_max = 1;          /* current sieve upper bound */

static void extend_primes(idx_t n) {
    if (n <= prime_mark_max) return;          /* already sieved up to n */

    idx_t old_limit = prime_mark_max;
    idx_t new_limit = old_limit ? old_limit * 2 : 4;
    while (new_limit < n) new_limit *= 2;

    /* allocate or grow the composite marker array */
    if (!prime_mark) {
        prime_mark = (unsigned char*)malloc(new_limit + 1);
        memset(prime_mark, 0, new_limit + 1);
        /* 0 and 1 are not prime */
        if (new_limit >= 0) prime_mark[0] = 1;
        if (new_limit >= 1) prime_mark[1] = 1;
    } else {
        prime_mark = (unsigned char*)realloc(prime_mark, new_limit + 1);
        memset(prime_mark + old_limit + 1, 0, new_limit - old_limit);
    }

    /* cross out composites using previously known primes */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t start = (old_limit + 1 + p - 1) / p * p;
        if (start < p * (idx_t)p) start = p * (idx_t)p;
        for (idx_t j = start; j <= new_limit; j += p)
            prime_mark[j] = 1;
    }

    /* identify new primes and cross out their multiples within the new range */
    for (idx_t i = old_limit + 1; i <= new_limit; ++i) {
        if (!prime_mark[i]) {
            /* i is prime */
            if (primes_cap <= primes_cnt) {
                primes_cap = primes_cap ? primes_cap * 2 : 64;
                primes = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_cnt++] = i;
            if (i * (idx_t)i <= new_limit) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    prime_mark[j] = 1;
            }
        }
    }
    prime_mark_max = new_limit;
}

/* binary search for first prime > n; result is number of primes ≤ n */
static idx_t upper_bound_prime_count(idx_t n) {
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= n) lo = mid + 1;
        else hi = mid;
    }
    return (idx_t)lo;
}

/* τ(n) using trial division on the prime list */
static seq_t tau_count(idx_t n) {
    idx_t temp = n;
    seq_t res = 1;
    for (size_t i = 0; i < primes_cnt && primes[i] * (idx_t)primes[i] <= temp; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            res *= (e + 1);
        }
    }
    if (temp > 1) res *= 2;   /* remaining prime factor */
    return res;
}

/* The sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;    /* undefined for n < 1, but returns 0 to keep behaviour sane */
    extend_primes(n);
    seq_t pi_val = (seq_t)upper_bound_prime_count(n);
    seq_t tau_val = tau_count(n);
    return pi_val + tau_val;
}
