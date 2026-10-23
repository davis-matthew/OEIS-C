/*
 * A025473  a(1) = 1; for n > 1, a(n) = prime root of n-th prime power
 *          (A000961).
 * https://oeis.org/A025473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = prime root of the n-th prime power (a(1) = 1; for n > 1,
 *            a(n) = p where p^k is the n-th smallest prime power)
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool _a_initialized = false;
static seq_t *_a_cache = NULL;
static size_t _a_cache_len = 0;

/* Small helper struct for sorting prime powers by value. */
typedef struct {
    uint64_t value;
    seq_t root;
} pp_t;

/* Comparator for qsort: ascending by value. */
static int pp_compare(const void *a, const void *b) {
    const pp_t *x = (const pp_t *)a;
    const pp_t *y = (const pp_t *)b;
    if (x->value < y->value) return -1;
    if (x->value > y->value) return 1;
    return 0; /* values are unique for prime powers */
}

/* Build cache of prime roots for prime powers up to LIMIT. */
static void _a_init(void) {
    if (_a_initialized) return;
    _a_initialized = true;

    /* Upper limit for prime generation – large enough for n ≤ 10000. */
    const uint64_t LIMIT = 200000;

    /* Simple sieve to find primes ≤ LIMIT. */
    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    size_t prime_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            ++prime_cnt;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }

    /* Allocate array of primes. */
    uint64_t *primes = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) primes[idx++] = i;
    }
    free(is_composite);

    /* Generate all prime powers ≤ LIMIT. */
    size_t cap = prime_cnt * 3; /* over‑estimate */
    pp_t *powers = (pp_t *)malloc(cap * sizeof(pp_t));
    size_t pcnt = 0;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t val = p;
        while (val <= LIMIT) {
            if (pcnt >= cap) {
                cap *= 2;
                powers = (pp_t *)realloc(powers, cap * sizeof(pp_t));
            }
            powers[pcnt].value = val;
            powers[pcnt].root  = (seq_t)p;
            ++pcnt;
            if (val > LIMIT / p) break; /* prevent overflow */
            val *= p;
        }
    }
    free(primes);

    /* Sort by value to obtain the correct order of prime powers. */
    qsort(powers, pcnt, sizeof(pp_t), pp_compare);

    /* Extract roots into cache. */
    _a_cache = (seq_t *)malloc(pcnt * sizeof(seq_t));
    for (size_t i = 0; i < pcnt; ++i) {
        _a_cache[i] = powers[i].root;
    }
    _a_cache_len = pcnt;
    free(powers);
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1; /* offset 1 */

    _a_init();

    idx_t pos = n - 2; /* zero‑based index in cache */
    if (pos >= _a_cache_len) {
        /* For safety when n exceeds pre‑computed range: */
        return (seq_t)0;
    }
    return _a_cache[pos];
}
