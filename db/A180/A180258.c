/*
 * A180258  a(n) = 2^(q*r) mod p for the n-th odd number with exactly 3
 *          distinct prime factors p < q < r.
 * https://oeis.org/A180258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A180258
   Formula: a(n) = 2^(q*r) mod p
   where p < q < r are the odd prime factors of the n-th odd integer
   that has exactly three distinct prime factors. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

typedef struct {
    uint64_t prod;
    uint32_t p;
    uint32_t q;
    uint32_t r;
} triple_t;

static seq_t *cache = NULL;
static size_t cache_len = 0;
static bool initialized = false;

static int cmp_triple(const void *a, const void *b)
{
    const triple_t *x = (const triple_t *)a;
    const triple_t *y = (const triple_t *)b;
    if (x->prod < y->prod) return -1;
    if (x->prod > y->prod) return 1;
    return 0;
}

static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Limit for generating numbers with 3 distinct odd prime factors.
       3,000,000 is enough to produce hundreds of thousands of such
       numbers, far exceeding the 10,000 terms required. */
    const uint64_t LIMIT = 3000000ULL;

    /* Sieve of Eratosthenes for primes up to LIMIT. */
    uint8_t *is_prime = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) return;
    for (uint64_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i) is_prime[j] = 0;
        }
    }

    /* Collect primes into a dynamic array. */
    uint32_t *primes = (uint32_t *)malloc((LIMIT / 10) * sizeof(uint32_t));
    size_t primes_cnt = 0, primes_cap = LIMIT / 10;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { free(primes); free(is_prime); return; }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }

    /* Allocate storage for triples. 1,000,000 is a safe upper bound. */
    triple_t *records = (triple_t *)malloc(1000000 * sizeof(triple_t));
    if (!records) { free(primes); free(is_prime); return; }
    size_t rec_cnt = 0, rec_cap = 1000000;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if (p == 2) continue;                 /* need odd primes only   */
        for (size_t j = i + 1; j < primes_cnt; ++j) {
            uint32_t q = primes[j];
            uint64_t prod_pq = (uint64_t)p * q;
            if (prod_pq > LIMIT) break;
            for (size_t k = j + 1; k < primes_cnt; ++k) {
                uint32_t r = primes[k];
                uint64_t prod = prod_pq * r;
                if (prod > LIMIT) break;
                if (rec_cnt == rec_cap) {
                    rec_cap *= 2;
                    triple_t *tmp = (triple_t *)realloc(records, rec_cap * sizeof(triple_t));
                    if (!tmp) { free(records); free(primes); free(is_prime); return; }
                    records = tmp;
                }
                records[rec_cnt].prod = prod;
                records[rec_cnt].p    = p;
                records[rec_cnt].q    = q;
                records[rec_cnt].r    = r;
                ++rec_cnt;
            }
        }
    }

    /* Sort triples by product to obtain the natural ordering of odd numbers
       with three distinct prime factors. */
    qsort(records, rec_cnt, sizeof(triple_t), cmp_triple);

    /* Allocate cache for the sequence values. */
    cache = (seq_t *)malloc(rec_cnt * sizeof(seq_t));
    if (!cache) { free(records); free(primes); free(is_prime); return; }
    cache_len = rec_cnt;

    /* Compute a(n) for each sorted number. */
    for (size_t idx = 0; idx < rec_cnt; ++idx) {
        const triple_t *t = &records[idx];
        uint64_t exp = (uint64_t)t->q * t->r;
        cache[idx] = mod_pow((seq_t)2, exp, (seq_t)t->p);
    }

    /* Free temporary storage. */
    free(records);
    free(primes);
    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init();
    if ((size_t)(n - 1) >= cache_len) return (seq_t)0;
    return cache[(size_t)(n - 1)];
}
