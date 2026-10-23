/*
 * A175169  Numbers k that divide the sum of digits of 2^k.
 * https://oeis.org/A175169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute OEIS sequence A175169: numbers k that divide the sum of digits of 2^k.
 * The function returns the n‑th term with offset 1.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define BASE 1000000000U  /* 1e9, fits in 32 bits */

seq_t a(idx_t n)
{
    /* static data for memoization */
    static idx_t curr_k = 1;             /* next k to test   */
    static seq_t *cache = NULL;          /* found k values   */
    static size_t cache_len = 0;         /* number of entries */
    static size_t cache_cap = 0;         /* allocated capacity */

    /* big integer in base BASE for 2^k */
    static uint32_t *big = NULL;         /* limbs, little endian */
    static size_t big_len = 0;           /* number of used limbs */
    static size_t big_cap = 0;           /* allocated capacity for limbs */

    /* one-time initialization */
    if (!cache_cap) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* alloc fail */
    }
    if (!big_cap) {
        big_cap = 8;
        big = (uint32_t *)malloc(big_cap * sizeof(uint32_t));
        if (!big) return 0;
        big[0] = 1;     /* 2^0 */
        big_len = 1;
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* multiply big integer by 2 to get next power of two */
        uint64_t carry = 0;
        for (size_t i = 0; i < big_len; ++i) {
            uint64_t tmp = (uint64_t)big[i] * 2 + carry;
            big[i]   = (uint32_t)(tmp % BASE);
            carry    = tmp / BASE;
        }
        if (carry > 0) {
            if (big_len == big_cap) {
                big_cap *= 2;
                big = (uint32_t *)realloc(big, big_cap * sizeof(uint32_t));
                if (!big) return 0;
            }
            big[big_len++] = (uint32_t)carry;
        }

        /* sum decimal digits of big modulo curr_k */
        idx_t modulo = curr_k;
        idx_t modsum = 0;
        for (size_t i = 0; i < big_len; ++i) {
            uint32_t limb = big[i];
            /* handle 0 limb explicitly */
            if (limb == 0 && i == big_len - 1) {
                modsum += 0;
                continue;
            }
            /* decompose limb into decimal digits */
            while (limb > 0) {
                modsum += limb % 10;
                if (modsum >= modulo) modsum %= modulo;
                limb /= 10;
            }
        }

        /* if divisible, store k */
        if (modsum == 0) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) return 0;
            }
            cache[cache_len++] = (seq_t)curr_k;
        }

        ++curr_k;  /* next k to test */
    }

    return cache[n - 1];
}
