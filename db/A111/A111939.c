/*
 * A111939  Number of primes < semiprime(n).
 * https://oeis.org/A111939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000720(A001358(n)) (number of primes < semiprime(n))
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8092 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Global data, initialized on first call */
static bool initialized = false;
static seq_t *prime_prefix = NULL;          /* prime counts up to LIMIT */
static seq_t *semiprimes = NULL;           /* sorted semiprimes up to LIMIT */
static idx_t semip_size = 0;

/* 2 000 000 is large enough to cover > 200 000 semiprimes, well beyond 10 000 */
#define LIMIT 2000000U

/* Helper: Sieve primes, build prime counts and semiprimes array */
static void init_sequence(void)
{
    /* Sieve of Eratosthenes */
    bool *is_composite = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= LIMIT; ++i) is_composite[i] = false;
    is_composite[0] = is_composite[1] = true;

    uint32_t *primes = (uint32_t *)malloc((LIMIT/10 + 10) * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    idx_t prime_cnt = 0;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * 2; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }

    /* Prime prefix counts */
    prime_prefix = seq_malloc((idx_t)LIMIT + 1);
    seq_t acc = 0;
    for (idx_t i = 0; i <= LIMIT; ++i) {
        if (!is_composite[i] && i >= 2) acc += 1;
        prime_prefix[i] = acc;
    }

    /* Generate semiprimes <= LIMIT */
    idx_t cap = 0;
    idx_t size = 0;
    seq_t *semip_arr = NULL;
    for (idx_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        for (idx_t j = i; j < prime_cnt; ++j) {
            uint32_t q = primes[j];
            uint64_t prod = (uint64_t)p * q;
            if (prod > LIMIT) break;
            push_seq(&semip_arr, &cap, &size, (seq_t)prod);
        }
    }

    /* Sort semiprimes */
    mem_qsort(semip_arr, size, sizeof(seq_t), cmp_seq);

    semiprimes = semip_arr;
    semip_size = size;

    free(is_composite);
    free(primes);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    /* sequence is 1‑based */
    if (n == 0 || n > semip_size) {
        /* Out of bounds: return 0 as a safe default */
        return (seq_t)0;
    }
    seq_t semiprime_val = semiprimes[n - 1];
    /* number of primes < semiprime_val */
    idx_t idx = (idx_t)(semiprime_val - 1);
    if (idx > (idx_t)LIMIT) idx = LIMIT;      /* safety guard */
    return prime_prefix[idx];
}
