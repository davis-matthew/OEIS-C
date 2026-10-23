/*
 * A190530  Numbers k such that 2^k - 3 is not squarefree.
 * https://oeis.org/A190530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A190530 – numbers k such that 2^k – 3 is not squarefree. *
 *            Computes the sequence by generating all k for which 2^k ≡ 3 (mod
 *            p²) * for primes p. For each prime p with a solution k₀, the set
 *            of such k is * the arithmetic progression k = k₀ + t·ord₂(p²) (t
 *            ≥ 0). The union of all * these progressions...
 * Range:     u8 none, u16 n <= 49, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   ord2_and_k0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ord2_and_k0.h"

/* Forward declaration required by build */
extern unsigned int _prime_limit_for_idx(idx_t n);
extern uint64_t _smallest_prime_factor(uint64_t n);

/* Helper for memory allocation (works for both C and C++ builds) */
static void *my_malloc(size_t n) {
    return malloc(n);
}

/* Holds a candidate progression from a prime */
typedef struct {
    seq_t next;   /* next value of the progression */
    seq_t step;   /* period (order of 2 modulo p²) */
} cand_t;

/* Prime sieve up to a fixed limit (2000 is enough for the first 10 000 terms) */
static void build_primes_upto_2000(uint32_t **primes_out, size_t *cnt_out) {
    const uint32_t LIMIT = 2000;
    bool *is_composite = (bool*)my_malloc((LIMIT + 1) * sizeof(bool));
    uint32_t *primes = (uint32_t*)my_malloc(LIMIT * sizeof(uint32_t));
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    *primes_out = primes;
    *cnt_out = cnt;
}

seq_t a(idx_t n) {
    /* Static data: primes, candidates, cache */
    static bool initialized = false;
    static size_t prime_cnt = 0;
    static uint32_t *primes = NULL;
    static size_t cand_cnt = 0;
    static cand_t *cand = NULL;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* First time: build primes up to 2000 and initialize candidates */
    if (!initialized) {
        build_primes_upto_2000(&primes, &prime_cnt);
        /* Determine for each prime if a solution exists and its period */
        cand = (cand_t*)my_malloc(prime_cnt * sizeof(cand_t));
        cand_cnt = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if (p <= 2) continue;            /* skip p = 2 */
            uint64_t period, k0;
            ord2_and_k0(p, &period, &k0);
            if (k0 == 0) continue;           /* no solution for this prime */
            cand[cand_cnt].next = k0;
            cand[cand_cnt].step = period;
            ++cand_cnt;
        }
        cache_cap = 128;
        cache = (seq_t*)my_malloc(cache_cap * sizeof(seq_t));
        initialized = true;
    }

    /* Ensure cache can hold up to n terms */
    idx_t target = n;
    while (cache_len < (size_t)target) {
        /* Find the smallest next value among all candidates */
        seq_t min_val = cand[0].next;
        size_t min_idx = 0;
        for (size_t i = 1; i < cand_cnt; ++i) {
            if (cand[i].next < min_val) {
                min_val = cand[i].next;
                min_idx = i;
            }
        }

        /* Skip duplicate values */
        if (cache_len > 0 && min_val == cache[cache_len - 1]) {
            cand[min_idx].next += cand[min_idx].step;
            continue;
        }

        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Store the new term and advance its candidate */
        cache[cache_len++] = min_val;
        cand[min_idx].next += cand[min_idx].step;
    }

    return cache[target - 1];
}
