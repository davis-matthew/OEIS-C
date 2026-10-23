/*
 * A020914  Number of digits in the base-2 representation of 3^n.
 * https://oeis.org/A020914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(1 + n*log₂(3)) – the number of bits of 3ⁿ
 * Range:     u8 n <= 160, u16 n <= 9936, u32 n <= 9936, u64 n <= 9936,
 *            u128 n <= 9936, big any n
 * Helpers:   msb_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/msb_bits.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;          /* a[0 .. computed] */
    static size_t cache_cap = 0;        /* allocated size of cache */
    static size_t cache_size = 0;       /* number of terms stored (≥1) */

    /* Big‑integer representation of 3ᶜᶜᶜ…  (little‑endian, base 2⁶⁴) */
    static uint64_t *big = NULL;
    static size_t big_cap = 0;          /* allocated size of big */
    static size_t big_len = 0;          /* number of used words of big */
    static idx_t current_exp = 0;       /* exponent whose value is stored in big */

    /* First call: initialise empty structures */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure – fallback */
    }
    if (big_cap == 0) {
        big_cap = 1;
        big = (uint64_t *)malloc(big_cap * sizeof(uint64_t));
        if (!big) return 0;
    }

    /* Extend cache up to n if required */
    if (n >= (idx_t)cache_size) {
        /* Ensure cache capacity */
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                 /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise the first term if cache empty */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            cache_size = 1;
            current_exp = 0;
            big_len = 1;
            big[0] = 1ULL;
        }

        /* Compute terms up to n */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) continue;              /* already initialised */

            /* Multiply big by 3 (big[0] is least significant word) */
            __uint128_t carry = 0;
            for (size_t j = 0; j < big_len; ++j) {
                __uint128_t prod = (__uint128_t)big[j] * 3 + carry;
                big[j] = (uint64_t)prod;
                carry = prod >> 64;
            }
            if (carry != 0) {
                if (big_len == big_cap) {
                    size_t new_cap = big_cap * 2;
                    uint64_t *tmp =
                        (uint64_t *)realloc(big, new_cap * sizeof(uint64_t));
                    if (!tmp) return 0;
                    big = tmp;
                    big_cap = new_cap;
                }
                big[big_len] = (uint64_t)carry;
                big_len++;
            }

            /* Determine bit length: msw is the most significant word */
            uint64_t msw = big[big_len - 1];
            int bits_msw = msb_bits(msw);
            seq_t len = (seq_t)((big_len - 1) * 64 + bits_msw);

            cache[i] = len;
        }
        cache_size = (size_t)n + 1;
        current_exp = n;
    }

    return cache[(size_t)n];
}
