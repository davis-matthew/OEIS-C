/*
 * A138077  Index of the largest maximal subgroup of PSL(3,q) as q runs
 *          through the prime powers (A000961).
 * https://oeis.org/A138077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = q^2 + q + 1, where q is the n-th prime power (A000961)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Global storage for prime powers */
static uint64_t *prime_power = NULL;   /* array of prime powers */
static size_t   prime_power_cnt = 0;   /* number of prime powers stored */
static uint64_t prime_power_limit = 0; /* current maximal value used to generate */

/* Comparator for qsort (uint64_t) */
static int cmp_uint64(const void *a, const void *b) {
    const uint64_t va = *(const uint64_t *)a;
    const uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

/* Build sorted list of all prime powers <= limit */
static void recompute_prime_powers(uint64_t limit) {
    /* Sieve of Eratosthenes */
    uint8_t *sieve = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!sieve) exit(1);
    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }

    /* Generate prime powers */
    size_t cap = 64;
    uint64_t *arr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!arr) exit(1);
    size_t cnt = 0;
    for (uint64_t p = 2; p <= limit; ++p) {
        if (!sieve[p]) continue;          /* p is prime */
        uint64_t val = p;
        while (val <= limit) {
            if (cnt == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(arr, cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                arr = tmp;
            }
            arr[cnt++] = val;
            if (limit / val < p) break;    /* avoid overflow on next multiplication */
            val *= p;
        }
    }

    /* Sort the array */
    qsort(arr, cnt, sizeof(uint64_t), cmp_uint64);

    /* Replace the global array */
    free(prime_power);
    prime_power = arr;
    prime_power_cnt = cnt;
    free(sieve);
}

/* Ensure we have at least n prime powers stored */
static void ensure_prime_powers(idx_t n) {
    if ((size_t)n <= prime_power_cnt) return;

    /* Start from an initial limit of 2 if not yet set */
    if (prime_power_limit < 2) prime_power_limit = 2;
    while ((size_t)n > prime_power_cnt) {
        prime_power_limit <<= 1;                 /* double the limit each time */
        recompute_prime_powers(prime_power_limit);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                /* offset is 1; behaviour for 0 is undefined */
    ensure_prime_powers(n);
    uint64_t q = prime_power[(size_t)(n - 1)];
    seq_t qs = (seq_t)q;
    return qs * qs + qs + (seq_t)1;             /* q^2 + q + 1 */
}
