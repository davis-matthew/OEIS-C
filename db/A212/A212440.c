/*
 * A212440  Numbers with even curling numbers of their binary representations,
 *          cf. A181935.
 * https://oeis.org/A212440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th number whose binary representation has an even
 *            curling number (curling number = largest c such that the binary
 *            string ends with c copies of some block)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9528 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute the curling number of a positive 64‑bit integer */
static int curling_number_uint64(uint64_t x)
{
    if (x == 0) return 0;
    int bits[64];
    int L = 0;
    uint64_t t = x;
    while (t) { t >>= 1; L++; }
    /* store bits from MSB to LSB */
    for (int i = 0; i < L; ++i)
        bits[i] = (int)((x >> (L - 1 - i)) & 1ULL);
    int maxc = 1;                     /* at least one repetition */
    for (int len = 1; len <= L; ++len) {    /* block length */
        int repeat = 1;
        int start = L - len;           /* start of last block */
        while (start - len >= 0) {
            bool ok = true;
            for (int k = 0; k < len; ++k) {
                if (bits[start - len + k] != bits[start + k]) { ok = false; break; }
            }
            if (!ok) break;
            repeat++;
            start -= len;
        }
        if (repeat > maxc) maxc = repeat;
    }
    return maxc;
}

/* Dynamic cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* External linkage function requested by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, behaviour for 0 is undefined */
    /* Ensure cache has at least n elements */
    while (cache_len < (size_t)n) {
        uint64_t cand = (uint64_t)(cache_len ? cache[cache_len - 1] : 0) + 1;
        /* Find next number with even curling number */
        while (true) {
            int c = curling_number_uint64(cand);
            if ((c & 1) == 0) {  /* even */
                grow_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)cand;
                break;
            }
            cand++;
        }
    }
    return cache[n - 1];
}
