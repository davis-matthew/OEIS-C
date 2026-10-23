/*
 * A123299  Prime sums of 13 positive 5th powers.
 * https://oeis.org/A123299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1924, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = the n-th prime that can be written as a sum of 13 positive 5th powers.
 *  Implementation:  Compute all representable sums by dynamic programming on a bitset,
 *  sieve primes up to a fixed limit, collect those primes whose offset (prime - 13)
 *  belongs to the representable set.
 *
 *  The function keeps a static cache so that repeated calls for any n ≥ 1
 *  are answered in O(1) after an initial cost of about 0.2 s on modern machines.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static const idx_t LIMIT = 20000000ULL;   /* sieve limit – enough for >10 000 terms */
static seq_t *prime_cache = NULL;         /* cache of representable primes */
static size_t cache_cnt = 0;              /* number of primes stored */

static inline bool bit_test(const uint64_t *bits, idx_t k)
{
    return (bits[k >> 6] >> (k & 63ull)) & 1ull;
}

static void build_sequence(void)
{
    /* 1. generate all 5th powers up to LIMIT */
    unsigned int inc_cnt = 0;
    {
        uint64_t base = 1;
        while (1) {
            uint64_t p5 = 1;
            for (int i = 0; i < 5; ++i) p5 *= base;
            if (p5 > LIMIT) break;
            ++base;
        }
        inc_cnt = (unsigned int)(base - 1);
    }

    uint64_t *inc = (uint64_t*)malloc(inc_cnt * sizeof(uint64_t));
    {
        uint64_t base = 1;
        unsigned int idx = 0;
        while (1) {
            uint64_t p5 = 1;
            for (int i = 0; i < 5; ++i) p5 *= base;
            if (p5 > LIMIT) break;
            inc[idx++] = p5 - 1;          /* increment value (k^5-1) */
            ++base;
        }
    }

    /* 2. DP: bitset of achievable sums of 13 increments */
    size_t words = (LIMIT >> 6) + 1;
    uint64_t *dp_cur = (uint64_t*)calloc(words, sizeof(uint64_t));
    uint64_t *dp_next = (uint64_t*)calloc(words, sizeof(uint64_t));
    dp_cur[0] = 1ull;                   /* sum 0 achievable with 0 dice */

    for (unsigned int dice = 0; dice < 13; ++dice) {
        memset(dp_next, 0, words * sizeof(uint64_t));
        for (unsigned int j = 0; j < inc_cnt; ++j) {
            uint64_t shift = inc[j];
            size_t wshift = shift >> 6;
            unsigned int bshift = shift & 63u;
            for (size_t i = 0; i < words; ++i) {
                uint64_t src = dp_cur[i];
                if (!src) continue;
                size_t dst = i + wshift;
                if (dst >= words) break;
                dp_next[dst] |= src << bshift;
                if (bshift && dst + 1 < words) dp_next[dst + 1] |= src >> (64 - bshift);
            }
        }
        uint64_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }
    free(dp_next);   /* dp_next is no longer needed */
    /* dp_cur now holds achievable sums of 13 increments (bits index = sum) */

    /* 3. Sieve primes up to LIMIT */
    unsigned char *composite = (unsigned char*)calloc(LIMIT + 1, 1);
    for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) composite[j] = 1;
        }
    }

    /* 4. Collect representable primes */
    size_t cap = 16384;           /* initial capacity; will grow as needed */
    prime_cache = (seq_t*)malloc(cap * sizeof(seq_t));
    cache_cnt = 0;

    for (idx_t p = 13; p <= LIMIT; ++p) {
        if (composite[p]) continue;      /* not prime */
        idx_t offset = p - 13;
        if (bit_test(dp_cur, offset)) {
            if (cache_cnt == cap) {
                cap <<= 1;
                prime_cache = (seq_t*)realloc(prime_cache, cap * sizeof(seq_t));
            }
            prime_cache[cache_cnt++] = (seq_t)p;
        }
    }

    free(dp_cur);
    free(inc);
    free(composite);
}

/* 5. Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined for n = 0 */
    if (prime_cache == NULL) build_sequence(); /* initialise once */
    if (n > cache_cnt) return (seq_t)0;       /* out of bounds */
    return prime_cache[n - 1];
}
