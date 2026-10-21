/*
 * A069781  Numbers k such that gcd(d(k^3), d(k)) is not a power of 2.
 * https://oeis.org/A069781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that gcd(d(k^3), d(k)) is not a
 *            power of 2, where d(m) is the number of positive divisors of m
 * Range:     u8 none, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"

static seq_t *seq     = NULL;   /* cached sequence values      */
static size_t seq_len = 0;      /* number of values stored     */
static size_t seq_cap = 0;      /* allocated capacity          */

static uint64_t *primes      = NULL; /* prime list for factorisation */
static size_t   primes_cnt   = 0;    /* number of primes stored      */
static uint64_t prime_limit  = 0;    /* maximum prime value stored   */

static uint64_t next_k = 1;  /* next integer to test   */

/* small primes up to 200 used to factor the very small numbers (e_i+1,
   3*e_i+1).  They are also used for marking the odd prime factors. */
static const uint16_t SMALL_PRIMES[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97,101,103,107,
    109,113,127,131,137,139,149,151,157,163,
    167,173,179,181,191,193,197,199
};
static const size_t SMALL_PRIME_COUNT =
    sizeof(SMALL_PRIMES) / sizeof(SMALL_PRIMES[0]);

/* map a prime <=200 to its index in SMALL_PRIMES; -1 if not present */
static int8_t prime_to_index[201];
static bool map_initialized = false;
static void init_prime_map(void)
{
    if (map_initialized) return;
    for (int i = 0; i < 201; ++i) prime_to_index[i] = -1;
    for (size_t i = 0; i < SMALL_PRIME_COUNT; ++i)
        prime_to_index[SMALL_PRIMES[i]] = (int8_t)i;
    map_initialized = true;
}

/* Ensure we have all primes <= limit in the global array.  Growing
   is done incrementally to avoid repeated allocations. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;
    if (primes) {
        free(primes);
        primes = NULL;
    }
    build_primes_upto(limit, &primes, &primes_cnt);
    prime_limit = limit;
}

/* Append a value to the sequence array, expanding it if needed. */
static void append_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 64 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) abort();
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* Factor a small number (<= ~200) and set bits for its odd prime
   factors into the supplied mask.  The mask uses bit i for SMALL_PRIMES[i]. */
static void factor_small(uint32_t v, uint64_t *mask)
{
    for (size_t i = 0; i < SMALL_PRIME_COUNT && (uint32_t)SMALL_PRIMES[i] * SMALL_PRIMES[i] <= v; ++i) {
        uint32_t p = SMALL_PRIMES[i];
        if (v % p == 0) {
            if (p > 2) *mask |= (1ULL << i);
            while (v % p == 0) v /= p;
        }
    }
    if (v > 1 && v != 2) {
        int idx = prime_to_index[v];
        if (idx >= 0) *mask |= (1ULL << idx);
    }
}

/* Return true if k satisfies the sequence condition. */
static bool is_member(uint64_t k)
{
    if (k <= 1) return false;          /* 1 is not in the sequence */
    init_prime_map();

    uint64_t mask1 = 0;   /* odd primes dividing some (e_i+1) */
    uint64_t mask2 = 0;   /* odd primes dividing some (3*e_i+1) */

    uint64_t tmp = k;
    uint64_t limit = int_sqrt(k);
    ensure_primes_upto(limit);

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        uint64_t cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        if (cnt > 0) {
            uint32_t ai = (uint32_t)(cnt + 1);
            uint32_t bi = (uint32_t)(3 * cnt + 1);
            factor_small(ai, &mask1);
            factor_small(bi, &mask2);
        }
    }

    if (tmp > 1) {
        /* remaining prime factor has exponent 1 */
        uint32_t ai = 2;
        uint32_t bi = 4;
        /* ai and bi are powers of two -> only factor 2, so ignore */
    }

    /* if odd prime factors intersect, gcd has an odd factor */
    return (mask1 & mask2) != 0;
}

/* Public function: return the nth element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for 0, but safe */

    while (seq_len < (size_t)n) {
        if (is_member(next_k))
            append_seq((seq_t)next_k);
        ++next_k;
    }
    return seq[n - 1];
}
