/*
 * A019495  Define the sequence T(a(0),a(1)) by a(n+2) is the greatest integer
 *          such that a(n+2)/a(n+1) < a(n+1)/a(n) for n >= 0. This is T(4,11).
 * https://oeis.org/A019495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   div256_by_128, mul128_u128, sub1_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A019495
 * a(0) = 4
 * a(1) = 11
 * a(n+2) = floor((a(n+1)^2 – 1) / a(n))   for n ≥ 0
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/div256_by_128.h"
#include "../utils/mul128_u128.h"
#include "../utils/sub1_u128.h"

#ifdef __cplusplus
/* In C++ the built‑in types are 128‑bit or a big‑int class,
   all of which can hold the square exactly. */
static inline seq_t next_term(seq_t prev, seq_t prev2)
{
    return (prev * prev - (seq_t)1) / prev2;
}
#else

static inline seq_t next_term(seq_t prev, seq_t prev2)
{
    __uint128_t low, high;
    mul128_u128(prev, prev, &low, &high);
    sub1_u128(&high, &low);
    return (seq_t)div256_by_128(high, low, (__uint128_t)prev2, NULL);
}
#endif

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    if (!cache)
    {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)4;
        cache[1] = (seq_t)11;
        len = 2;
    }

    while (len <= n)
    {
        if (len >= cap)
        {
            cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }

        seq_t prev  = cache[len - 1];
        seq_t prev2 = cache[len - 2];
        cache[len] = next_term(prev, prev2);
        len++;
    }

    return cache[(size_t)n];
}
