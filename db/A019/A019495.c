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
 * Validated: standard, pass, 44 terms through n = 804 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* 256‑bit by 128‑bit division.  Quotient fits into 128‑bits. */
static inline __uint128_t div256_by_128(__uint128_t high, __uint128_t low, __uint128_t d,
                                        __uint128_t *rem)
{
    __uint128_t quotient = 0;
    __uint128_t remainder = 0;

    for (int i = 255; i >= 0; --i)
    {
        remainder <<= 1;
        unsigned bit = (i >= 128) ?
                       (unsigned)((high >> (i - 128)) & 1) :
                       (unsigned)((low >> i) & 1);
        remainder |= bit;

        if (remainder >= d)
        {
            remainder -= d;
            if (i < 128)
                quotient |= ((__uint128_t)1 << i);
        }
    }
    if (rem) *rem = remainder;
    return quotient;
}

/* In C we must avoid overflow.  All built‑in seq_t types
   are ≤ 128 bits, so we represent numbers as 64‑bit halves
   and perform 256‑bit multiplication and 256/128 division. */
static inline void mul128_u128(seq_t a, seq_t b, __uint128_t *lo, __uint128_t *hi)
{
    uint64_t a0 = (uint64_t)a;
    uint64_t a1 = (uint64_t)(a >> 64);
    uint64_t b0 = (uint64_t)b;
    uint64_t b1 = (uint64_t)(b >> 64);

    __uint128_t p00 = (__uint128_t)a0 * b0;           /* low ⊗ low */
    __uint128_t p01 = (__uint128_t)a0 * b1;           /* low ⊗ high */
    __uint128_t p10 = (__uint128_t)a1 * b0;           /* high ⊗ low */
    __uint128_t p11 = (__uint128_t)a1 * b1;           /* high ⊗ high */

    __uint128_t cross = p01 + p10;
    uint64_t cross_low  = (uint64_t)cross;
    uint64_t cross_high = (uint64_t)(cross >> 64);

    __uint128_t low_part  = p00 + ((__uint128_t)cross_low << 64);
    __uint128_t carry     = (low_part < p00) ? 1 : 0;

    __uint128_t high_part = p11 + cross_high + carry;

    *lo = low_part;
    *hi = high_part;
}

/* Subtract 1 from a 256‑bit number encoded as (high, low). */
static inline void sub1_u128(__uint128_t *high, __uint128_t *low)
{
    if (*low == 0)
    {
        *low = (__uint128_t)(-1);
        (*high)--;
    }
    else
        (*low)--;
}

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
        cache = mem_new(seq_t, cap);
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
            seq_t *tmp = mem_resize(seq_t, cache, cap);
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
