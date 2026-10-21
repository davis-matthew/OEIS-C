/*
 * A238504  Number of primes p <= n with pi(pi((p-1)*n)) prime, where pi(x)
 *          denotes the number of primes not exceeding x.
 * https://oeis.org/A238504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 996, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A238504: Number of primes p ≤ n such that π(π((p-1)*n)) is prime.
   The function uses a sieve up to 100 000 000, which is sufficient for
   n ≤ 10 000 (because (p-1)*n ≤ (n-1)*n < 100 000 000).  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  constants  ---------- */
#define LIMIT 100000000ULL   /* maximum value needed for (p-1)*n */

/* ----------  global data  ---------- */
static bool initialized = false;
static uint64_t *bits = NULL;         /* odd‑number bitset: 1 = prime candidate */
static uint32_t *prefix = NULL;       /* prefix popcount per word */
static uint32_t *primes = NULL;       /* list of primes up to LIMIT */
static size_t primes_count = 0;       /* number of primes stored */
static size_t primes_cap = 0;         /* capacity of primes[] */
static size_t bits_len = 0;           /* number of odd indices (3 … LIMIT) */
static size_t word_len = 0;           /* number of 64‑bit words in bits */

/* ----------  helper: grow primes array  ---------- */
static void grow_primes(size_t need)
{
    if (need <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = new_cap;
}

/* ----------  helper: integer sqrt (for LIMIT ≤ 1e8)  ---------- */
static size_t isqrt_u64(uint64_t v)
{
    size_t r = 0;
    while ((uint64_t)(r + 1) * (r + 1) <= v) ++r;
    return r;
}

/* ----------  sieve + prefix + prime list ---------- */
static void init_sieve(void)
{
    if (initialized) return;

    /* 1. number of odd indices 3 … LIMIT */
    bits_len = ((LIMIT - 3) >> 1) + 1;          /* (LIMIT−3)/2 + 1 */
    word_len = (bits_len + 63) >> 6;            /* words of 64 bits */

    /* 2. allocate bitset, all 1 initially (candidate primes) */
    bits = (uint64_t *)calloc(word_len, sizeof(uint64_t));
    if (!bits) exit(EXIT_FAILURE);
    for (size_t i = 0; i < word_len; ++i) bits[i] = ~0ULL;

    /* 3. allocate primes array, store 2 first */
    primes_cap = 1000;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    primes[0] = 2;
    primes_count = 1;

    /* 4. sieve odd numbers */
    size_t limit_sqrt = isqrt_u64(LIMIT);
    for (size_t idx = 0; idx < bits_len; ++idx) {
        if (bits[idx >> 6] & (1ULL << (idx & 63))) {
            uint32_t p = 3 + (uint32_t)(idx << 1);  /* current odd prime */
            grow_primes(primes_count + 1);
            primes[primes_count++] = p;
            if (p <= limit_sqrt) {
                uint64_t start = ((uint64_t)p * p - 3) >> 1;
                for (uint64_t j = start; j < bits_len; j += p)
                    bits[j >> 6] &= ~(1ULL << (j & 63));
            }
        }
    }

    /* 5. build prefix popcount array */
    prefix = (uint32_t *)malloc((word_len + 1) * sizeof(uint32_t));
    if (!prefix) exit(EXIT_FAILURE);
    prefix[0] = 0;
    for (size_t i = 0; i < word_len; ++i)
        prefix[i + 1] = prefix[i] + __builtin_popcountll(bits[i]);

    initialized = true;
}

/* ----------  is prime for n ≤ LIMIT  ---------- */
static inline bool is_prime_small(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    idx_t idx = (n - 3) >> 1;
    if ((size_t)idx >= bits_len) return false; /* outside sieve, composite */
    return (bits[idx >> 6] >> (idx & 63)) & 1ULL;
}

/* ----------  π(x) for x ≤ LIMIT  ---------- */
static inline seq_t pi_idx(idx_t x)
{
    if (x < 2) return 0;
    seq_t cnt = 1;   /* prime 2 */
    if (x >= 3) {
        idx_t k = (x - 3) >> 1;           /* last odd index ≤ x */
        idx_t word = k >> 6;
        idx_t bits_in_word = (k & 63) + 1;
        cnt += prefix[word];
        if (bits_in_word) {
            uint64_t mask = (bits_in_word == 64) ? ~0ULL
                                                 : ((1ULL << bits_in_word) - 1);
            cnt += __builtin_popcountll(bits[word] & mask);
        }
    }
    return cnt;
}

/* ----------  sequence function  ---------- */
seq_t a(idx_t n)
{
    init_sieve();
    seq_t result = 0;

    for (size_t i = 0; i < primes_count && primes[i] <= n; ++i) {
        idx_t p = primes[i];
        idx_t m = (p - 1) * n;                    /* safe for n ≤ 10000 */
        seq_t pi_m = pi_idx(m);                    /* π((p‑1)·n) */
        seq_t pi_pi_m = pi_idx((idx_t)pi_m);       /* π(π((p‑1)·n)) */
        if (is_prime_small((idx_t)pi_pi_m)) ++result;
    }

    return result;
}
