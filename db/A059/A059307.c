/*
 * A059307  Numbers n such that base 10 representation of n does not contain a
 *          block of digits which is a square: blocks like 0, 1, 4, 9, 36,
 *          121, ... are forbidden.
 * https://oeis.org/A059307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose decimal representation
 *            contains no contiguous block that is a perfect square (OEIS
 *            A059307). The function searches integers in increasing order,
 *            keeps a cache of already found terms, and returns the term at
 *            the requested index
 * Range:     u8 n <= 50, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* compute floor(sqrt(v)) for v <= 2^64-1 using a bit‑wise algorithm */
static inline uint64_t isqrt64(uint64_t v)
{
    uint64_t r = 0, bit = 1ULL << 62;                 /* 2^62 is highest bit <= sqrt(UINT64_MAX) */
    while (bit > v) bit >>= 2;
    while (bit != 0) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* test whether x has any contiguous decimal block that is a perfect square */
static bool has_no_square_block(seq_t x)
{
    /* determine number of decimal digits */
    seq_t tmp = x;
    size_t dlen = 0;
    while (tmp > 0) {
        tmp /= 10;
        dlen++;
    }
    if (dlen == 0) return true;                      /* should not happen for positive x */

    /* store digits most significant first */
    uint8_t *dig = (uint8_t*)calloc(dlen, sizeof(uint8_t));
    if (!dig) abort();                               /* allocation failed */
    tmp = x;
    for (size_t i = dlen; i > 0; --i) {
        dig[i-1] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    bool ok = true;
    for (size_t i = 0; i < dlen && ok; ++i) {
        uint64_t val = 0;
        for (size_t j = i; j < dlen; ++j) {
            val = val * 10 + dig[j];
            uint64_t r = isqrt64(val);
            if (r * r == val) {                    /* found a square block */
                ok = false;
                break;
            }
        }
    }

    free(dig);
    return ok;
}

/* --- sequence cache ------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 1;                       /* largest number examined */

/* --- required sequence function -------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                            /* undefined for n < 1, but return 0 */

    while (cache_len < (size_t)n) {
        seq_t candidate = last_checked + 1;
        last_checked = candidate;

        if (has_no_square_block(candidate)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
    }
    return cache[n - 1];
}
