/*
 * A075811  Palindromic even squarefree numbers with an even number of
 *          distinct prime factors.
 * https://oeis.org/A075811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   count_distinct_primes, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates palindromic even numbers and keeps only
   those that are square‑free and have an even number of distinct prime
   factors.  The function a(idx_t n) returns the nth such number
   (OEIS A075811).  The generator works independently of the call
   order and caches results in a static array. */
/* Include required utilities */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"

/* Powers of ten up to 10^10 (sufficient for all n up to ~10⁴). */
static const uint64_t pow10[11] = {
    1ULL,
    10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL, 100000000ULL,
    1000000000ULL, 10000000000ULL
};

static seq_t build_pal(uint64_t half, unsigned len)
{
    const unsigned halfLen = (len + 1) / 2;
    /* digits[0] is most significant */
    unsigned digits[10];
    uint64_t tmp = half;
    for (int i = halfLen - 1; i >= 0; --i) {
        digits[i] = tmp % 10;
        tmp /= 10;
    }
    seq_t pal = 0;
    for (unsigned i = 0; i < halfLen; ++i)
        pal = pal * 10 + digits[i];
    unsigned start = (len & 1) ? 1 : 0; /* skip middle digit */
    for (int i = halfLen - 1 - start; i >= 0; --i)
        pal = pal * 10 + digits[i];
    return pal;
}

static void ensure_capacity(size_t needed, size_t *cap, seq_t **p)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
    *p = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Generation state */
    static unsigned gen_len = 1;          /* current palindrome length */
    static unsigned even_idx = 0;         /* index in {2,4,6,8} */
    static uint64_t inner = 0;            /* variation part of the half */
    static const uint64_t evens[4] = {2,4,6,8};

    /* Generate until we have at least n cached */
    while (cache_len < n) {
        const unsigned halfLen = (gen_len + 1) / 2;
        const uint64_t limit = pow10[halfLen - 1];

        uint64_t half = evens[even_idx] * pow10[halfLen - 1] + inner;
        seq_t cand = build_pal(half, gen_len);

        /* advance state */
        ++inner;
        if (inner >= limit) {
            inner = 0;
            ++even_idx;
            if (even_idx >= 4) {
                even_idx = 0;
                ++gen_len;
            }
        }

        /* filter */
        if (is_squarefree(cand)) {
            int distinct = count_distinct_primes((uint64_t)cand);
            if ((distinct & 1) == 0) {
                ensure_capacity(cache_len + 1, &cache_cap, &cache);
                cache[cache_len++] = cand;
            }
        }
    }

    return cache[n - 1];
}
