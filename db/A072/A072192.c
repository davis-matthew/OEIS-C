/*
 * A072192  Indices of Sophie Germain primes: p and 2p+1 are primes.
 * https://oeis.org/A072192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the index of the n-th Sophie Germain prime in the
 *            sequence of all primes (i.e. a(n) = prime_index(SG_prime(n)) =
 *            A000720(A005384(n)) ) The implementation precomputes primes up
 *            to a fixed limit that guarantees at least 20000 Sophie Germain
 *            primes (sufficient for n ≤ 10000). It then extracts...
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime64.h"

static const idx_t INITIAL_LIMIT = 5000000;   /* 5 million – > 20000 SG primes */
static uint8_t *is_composite = NULL;          /* sieve bit array */
static uint32_t *primes = NULL;                /* list of primes */
static size_t primes_cnt = 0, primes_cap = 0;
static uint32_t *sg_idx = NULL;               /* indices of SG primes in primes list */
static size_t sg_cnt = 0, sg_cap = 0;
static bool initialized = false;

/* Sieve of Eratosthenes up to 'limit', fill primes[] and sg_idx[] */
static void compute_primes_and_sg(idx_t limit)
{
    if (is_composite) free(is_composite);
    is_composite = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!is_composite) exit(1);                /* allocation failure */

    primes_cnt = 0;  primes_cap = 0;
    if (primes) free(primes);
    primes = NULL;
    sg_cnt = 0;      sg_cap = 0;
    if (sg_idx) free(sg_idx);
    sg_idx = NULL;

    /* simple sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[(size_t)j] = 1;
        }
    }

    /* collect primes */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            if (primes_cnt >= primes_cap) {
                primes_cap = (primes_cap == 0) ? 1024 : primes_cap << 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }

    /* collect indices of Sophie Germain primes */
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint32_t p = primes[idx];
        uint64_t up = (uint64_t)p << 1;   /* 2p */
        up += 1;                          /* 2p+1 */
        if (is_prime64(up)) {
            if (sg_cnt >= sg_cap) {
                sg_cap = (sg_cap == 0) ? 1024 : sg_cap << 1;
                sg_idx = (uint32_t *)realloc(sg_idx, sg_cap * sizeof(uint32_t));
                if (!sg_idx) exit(1);
            }
            sg_idx[sg_cnt++] = (uint32_t)(idx + 1);  /* 1‑based position in primes list */
        }
    }
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe */

    if (!initialized) {
        compute_primes_and_sg(INITIAL_LIMIT);
        initialized = true;
    }

    /* If requested n is beyond precomputed capacity, extend (unlikely) */
    if ((idx_t)n > sg_cnt) {
        idx_t new_limit = INITIAL_LIMIT;
        while ((idx_t)n > sg_cnt) {
            new_limit <<= 1;          /* double the limit */
        }
        compute_primes_and_sg(new_limit);
    }

    return (seq_t)sg_idx[n - 1];
}
