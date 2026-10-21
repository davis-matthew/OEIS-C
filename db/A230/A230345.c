/*
 * A230345  Number of permutations of [2n+5] in which the longest increasing
 *          run has length n+5.
 * https://oeis.org/A230345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3+12*n^2+42*n+41)*(2*n+5)!/(n+7)! for n>0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve up to 1e6 (covers sqrt of maximum polynomial value for n<=10000) */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_prime_sieve(void)
{
    if (prime_count > 0) return;              /* already initialized */
    const idx_t LIMIT = 1000000;
    bool *sieve = (bool*)calloc((size_t)LIMIT + 1, 1);
    if (!sieve) abort();

    /* simple Eratosthenes */
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;

    /* collect primes */
    size_t cap = 1024;
    prime_list = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!prime_list) abort();

    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) {
            if (prime_count == cap) {
                cap <<= 1;
                prime_list = (uint32_t*)realloc(prime_list, cap * sizeof(uint32_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = (uint32_t)i;
        }

    free(sieve);
}

/* Legendre's formula: exponent of prime p in n! */
static idx_t exp_in_fact(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Static array for temporary exponent storage; will be allocated on first call */
static int *exp_arr = NULL;
static idx_t exp_arr_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    init_prime_sieve();

    /* polynomial part */
    seq_t n_sq = (seq_t)n * n;
    seq_t n_cu = n_sq * n;
    seq_t poly = n_cu + (seq_t)12 * n_sq + (seq_t)42 * (seq_t)n + 41;

    /* ensure exponent array large enough */
    if (exp_arr_size < prime_count) {
        exp_arr = (int*)realloc(exp_arr, prime_count * sizeof(int));
        if (!exp_arr) abort();
        exp_arr_size = prime_count;
    }

    /* reset exponents */
    for (idx_t i = 0; i < prime_count; ++i) exp_arr[i] = 0;

    /* add factorial ratio exponents: (2n+5)! / (n+7)! */
    const idx_t m1 = 2 * n + 5;
    const idx_t m2 = n + 7;
    for (idx_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        idx_t e = exp_in_fact(m1, p);
        e -= exp_in_fact(m2, p);
        exp_arr[i] = (int)e;   /* may be negative */
    }

    /* factor the polynomial and add exponents */
    seq_t val = poly;
    for (idx_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((seq_t)p * (seq_t)p > val) break;
        int cnt = 0;
        while (val % p == 0) {
            val /= p;
            ++cnt;
        }
        exp_arr[i] += cnt;
    }

    /* if a prime factor remains (> 1), it is a single large prime */
    bool large_prime = false;
    seq_t large_val = 0;
    if (val > 1) {
        large_prime = true;
        large_val = val;
    }

    /* multiply all prime powers (positive exponents only) */
    seq_t result = 1;
    for (idx_t i = 0; i < prime_count; ++i) {
        int e = exp_arr[i];
        if (e <= 0) continue;
        uint32_t p = prime_list[i];
        for (int k = 0; k < e; ++k)
            result = result * (seq_t)p;
    }

    if (large_prime) {
        result = result * large_val;
    }

    return result;
}
