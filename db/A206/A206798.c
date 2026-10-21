/*
 * A206798  Beginning with the natural numbers, swap the k-th composite number
 *          (A002808) and k-th noncomposite number (A008578), for k =
 *          1,2,3,...
 * https://oeis.org/A206798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = kth composite if n is noncomposite (prime or 1), else kth
 *            noncomposite (noncomposite numbers are 1 and all primes). The
 *            kth composite is the kth number ≥4 that is not prime; the kth
 *            noncomposite is 1 for k=1 and the (k‑1)th prime for k≥2. The
 *            function swaps these pairs
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Global state for sieving, prefix prime counts, and the two lists. */
static size_t sieve_limit      = 0;
static uint8_t *is_prime_small = NULL;   /* 0/1 array */
static uint32_t *pi_vec        = NULL;   /* pi_vec[i] = number of primes ≤ i */
static uint64_t *primes        = NULL;   /* primes[0] = 2, ... */
static size_t     primes_cnt   = 0;
static size_t     primes_cap   = 0;
static uint64_t *composites    = NULL;   /* composites[0] = 4, ... */
static size_t     composites_cnt = 0;
static size_t     composites_cap = 0;

/* Push a value into a dynamic uint64_t array, reallocating if needed. */
static void push_uint64(uint64_t **arr, size_t *cnt, size_t *cap, uint64_t val) {
    if (*cnt == *cap) {
        *cap = (*cap) ? (*cap) * 2 : 16;
        *arr = (uint64_t*)realloc(*arr, (*cap) * sizeof(uint64_t));
    }
    (*arr)[*cnt] = val;
    (*cnt)++;
}

/* Build a sieve up to "limit" (inclusive), allocating and filling all arrays. */
static void init_sieve(size_t limit) {
    if (limit <= sieve_limit) return;

    /* Free any previous data. */
    if (is_prime_small) free(is_prime_small);
    if (pi_vec)         free(pi_vec);
    if (primes)         free(primes);
    if (composites)     free(composites);

    /* Allocate the sieve array and prefix pi vector. */
    is_prime_small = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    pi_vec         = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!is_prime_small || !pi_vec) exit(1);

    memset(is_prime_small, 1, limit + 1);
    is_prime_small[0] = is_prime_small[1] = 0;

    /* Classic sieve of Eratosthenes. */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime_small[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_prime_small[j] = 0;
            }
        }
    }

    /* Reset lists and counters. */
    primes        = NULL; primes_cnt = 0; primes_cap = 0;
    composites    = NULL; composites_cnt = 0; composites_cap = 0;

    /* Build primes list, composites list, and prefix prime counts. */
    uint32_t pi = 0;
    pi_vec[0] = 0; pi_vec[1] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime_small[i]) {
            push_uint64(&primes, &primes_cnt, &primes_cap, i);
            ++pi;
        }
        if (i >= 4 && !is_prime_small[i]) {
            push_uint64(&composites, &composites_cnt, &composites_cap, i);
        }
        pi_vec[i] = pi;
    }
    sieve_limit = limit;
}

/* Ensure the sieve covers at least up to "value". */
static void ensure_sieve_upto(size_t value) {
    if (sieve_limit < value) {
        size_t new_limit = value * 2;
        if (new_limit < 20000) new_limit = 20000;
        init_sieve(new_limit);
    }
}

/* Ensure we have at least "k" composites stored. */
static void ensure_composites_k(size_t k) {
    while (composites_cnt < k) {
        size_t new_limit = sieve_limit * 2 + 100;
        init_sieve(new_limit);
    }
}

/* Ensure we have at least "k" primes stored. */
static void ensure_primes_k(size_t k) {
    while (primes_cnt < k) {
        size_t new_limit = sieve_limit * 2 + 100;
        init_sieve(new_limit);
    }
}

seq_t a(idx_t n) {
    /* Ensure sieve is large enough. */
    ensure_sieve_upto((size_t)n);

    uint32_t pi_n = pi_vec[(size_t)n];

    /* Noncomposite numbers: 1 and all primes. */
    if (n == 1 || (n >= 2 && is_prime_small[(size_t)n])) {
        size_t k = (size_t)pi_n + 1;            /* rank in noncomposite list */
        ensure_composites_k(k);
        uint64_t val = composites[k - 1];        /* kth composite */
        return (seq_t)val;
    } else { /* composite number */
        size_t k = (size_t)n - ((size_t)pi_n + 1); /* rank in composite list */
        ensure_primes_k(k);
        if (k == 1) {                           /* first noncomposite is 1 */
            return (seq_t)1;
        } else {                                 /* noncomposite_k = prime_{k-2} */
            uint64_t val = primes[k - 2];
            return (seq_t)val;
        }
    }
}
