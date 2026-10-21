/*
 * A097216  Numbers n such that A076078(n) > n, where A076078(n) equals the
 *          number of sets of distinct positive integers with a least common
 *          multiple of n.
 * https://oeis.org/A097216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   gen_masks_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that A076078(n) > n (OEIS A097216) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gen_masks_rec.h"

/* ---------- prime sieve (used for factorisation) ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_count > 0)
        return;                    /* already initialised */

    /* Sieve up to an upper bound.  1 000 000 is enough for the
       values that occur in the first few ten-thousand terms. */
    const size_t N = 1000000;
    bool *isprime = (bool *)malloc((N + 1u) * sizeof(bool));
    if (!isprime)
        exit(EXIT_FAILURE);
    for (size_t i = 0; i <= N; ++i)
        isprime[i] = true;
    isprime[0] = isprime[1] = false;

    for (size_t i = 2; i * i <= N; ++i) {
        if (isprime[i]) {
            for (size_t j = i * i; j <= N; j += i)
                isprime[j] = false;
        }
    }

    prime_count = 0;
    for (size_t i = 2; i <= N; ++i)
        if (isprime[i])
            ++prime_count;

    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list)
        exit(EXIT_FAILURE);
    size_t idx = 0;
    for (size_t i = 2; i <= N; ++i)
        if (isprime[i])
            prime_list[idx++] = (uint32_t)i;

    free(isprime);
}

/* ---------- count whether A076078(n) > n ---------- */
static bool property(uint64_t n)
{
    if (n <= 1)           /* A076078(1) = 1, 1 > 1 is false */
        return false;

    init_primes();

    /* factorise n */
    uint32_t p[16]; /* enough for the range we need */
    int    e[16];
    size_t k = 0;
    uint64_t t = n;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= t; ++i) {
        uint64_t pr = (uint64_t)prime_list[i];
        if (t % pr == 0) {
            int cnt = 0;
            do {
                t /= pr;
                ++cnt;
            } while (t % pr == 0);
            p[k] = (uint32_t)pr;
            e[k] = cnt;
            ++k;
        }
    }
    if (t > 1) {
        p[k] = (uint32_t)t;
        e[k] = 1;
        ++k;
    }

    /* generate masks for all divisors */
    static uint32_t masks[4096];   /* safe upper bound for the numbers we search */
    unsigned mcnt = 0;
    gen_masks_rec(0, (unsigned)k, e, 0u, masks, &mcnt);

    /* DP over masks (bitmask of primes that have already been covered) */
    const unsigned fullmask = (k == 0) ? 0u : ((1u << k) - 1u);
    const unsigned maxmask  = 1u << k;
    const uint64_t limit = n;
    const uint64_t cap   = (limit == UINT64_MAX) ? limit : (limit + 1);

    /* static storage for DP arrays */
    static uint64_t dp_storage[1 << 16];
    static uint64_t dp2_storage[1 << 16];
    uint64_t *dp  = dp_storage;
    uint64_t *dp2 = dp2_storage;

    memset(dp, 0, maxmask * sizeof(uint64_t));
    memset(dp2, 0, maxmask * sizeof(uint64_t));
    dp[0] = 1;

    for (unsigned i = 0; i < mcnt; ++i) {
        uint32_t m = masks[i];
        memcpy(dp2, dp, maxmask * sizeof(uint64_t));   /* dp2 = dp (subsets that skip this divisor) */

        for (unsigned j = 0; j < maxmask; ++j) {
            uint64_t val = dp[j];
            if (val == 0)
                continue;
            unsigned nm = j | m;
            uint64_t sum;
            if (dp2[nm] >= cap - val)
                sum = cap;
            else
                sum = dp2[nm] + val;
            dp2[nm] = sum;
        }

        /* swap dp and dp2 for next iteration */
        uint64_t *tmp = dp; dp = dp2; dp2 = tmp;
    }

    uint64_t f = dp[fullmask];
    return f > limit;
}

/* ---------- sequence cache ---------- */
static seq_t *seq_cache  = NULL;
static idx_t  cache_len  = 0;
static uint64_t next_val = 1;

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for offset 1 */

    while (cache_len < n) {
        if (property(next_val)) {          /* next important integer */
            seq_t *tmp = (seq_t *)realloc(seq_cache,
                                           (cache_len + 1) * sizeof(seq_t));
            if (!tmp) {
                free(seq_cache);
                exit(EXIT_FAILURE);
            }
            seq_cache = tmp;
            seq_cache[cache_len++] = (seq_t)next_val;
        }
        ++next_val;
    }
    return seq_cache[n - 1];
}
