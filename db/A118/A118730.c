/*
 * A118730  Numbers k such that 2^k has even digit sum.
 * https://oeis.org/A118730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9821 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 2^k has even digit sum */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint8_t *dec = NULL;
static size_t dec_len = 0;
static size_t dec_cap = 0;
static idx_t next_k = 1;
static bool initialized = false;

static inline void ensure_dec_cap(size_t needed)
{
    if (needed > dec_cap) {
        size_t new_cap = dec_cap ? dec_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(dec, new_cap * sizeof(uint8_t));
        dec = tmp;
        dec_cap = new_cap;
    }
}

static void dec_mul2(void)
{
    size_t i;
    uint16_t carry = 0;
    for (i = 0; i < dec_len; ++i) {
        uint16_t prod = (uint16_t)dec[i] * 2 + carry;
        dec[i] = prod % 10;
        carry   = prod / 10;
    }
    if (carry) {
        ensure_dec_cap(dec_len + 1);
        dec[dec_len++] = carry;
    }
}

static uint8_t dec_digit_sum(void)
{
    uint8_t sum = 0;
    size_t i;
    for (i = 0; i < dec_len; ++i)
        sum += dec[i];
    return sum;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Allocate minimal cache */
        ensure_capacity(4, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache_len = 0;

        /* Decimal representation of 2^1 = 2 */
        ensure_dec_cap(1);
        dec[0] = 2;
        dec_len = 1;
        next_k = 1;
        initialized = true;
    }

    while (cache_len < n) {
        /* dec currently holds 2^next_k */
        if ((dec_digit_sum() & 1) == 0) {
            if (cache_len == cache_cap)
                ensure_capacity(cache_cap ? cache_cap * 2 : 4, &cache_cap,
                                OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
        dec_mul2();
    }

    return cache[n - 1];
}
