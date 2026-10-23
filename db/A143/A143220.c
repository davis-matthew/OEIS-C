/*
 * A143220  a(0)=1. For n >=1, a(n) = 1 if the binary representation of n
 *          occurs at least once in the concatenation of
 *          (a(0),a(1),...,a(n-1)). a(n) = 0 otherwise.
 * https://oeis.org/A143220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 if the binary representation of n occurs in the
 *            concatenation of a(0),a(1),...,a(n-1); 0 otherwise
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   get_bin_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation stores the sequence in a cache and the
   cumulative bit string in a separate uint8_t buffer.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_bin_pattern.h"

/* ---------- global state ---------- */
static bool         initialized      = false;
static seq_t       *cache            = NULL;   /* sequence values      */
static size_t       cache_cap        = 0;      /* allocated cache size */
static uint8_t      *bits             = NULL;   /* concatenated bits    */
static size_t       bits_len         = 0;      /* current bit count    */
static size_t       bits_cap         = 0;      /* allocated bits size  */
static idx_t        computed_up_to   = 0;      /* highest index computed */

/* ---------- helper functions ---------- */

/* ensure cache has at least 'need' elements */
static inline void ensure_cache_cap(size_t need)
{
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < need) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* ensure bits array has at least 'need' slots */
static inline void ensure_bits_cap(size_t need)
{
    if (need > bits_cap) {
        size_t new_cap = bits_cap ? bits_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        uint8_t *tmp = (uint8_t*)realloc(bits, new_cap);
        bits = tmp;
        bits_cap = new_cap;
    }
}

/* append a single bit ('0' or '1') to the cumulative bit string */
static inline void append_bit(char bit)
{
    if (bits_len == bits_cap) {
        size_t new_cap = bits_cap ? bits_cap * 2 : 8;
        uint8_t *tmp = (uint8_t*)realloc(bits, new_cap);
        bits = tmp;
        bits_cap = new_cap;
    }
    bits[bits_len++] = bit;
}

/* ---------- sequence generator ---------- */
seq_t a(idx_t n)
{
    /* first‑time initialization */
    if (!initialized) {
        initialized = true;

        /* cache for a(0) */
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;

        /* bits array */
        bits_cap = 8;
        bits = (uint8_t*)malloc(bits_cap);
        bits_len = 0;
        append_bit('1');          /* a(0) = 1 */
        computed_up_to = 0;
    }

    /* if we need more terms */
    if (n > computed_up_to) {
        /* enlarge storage first */
        ensure_cache_cap((size_t)(n + 1));
        ensure_bits_cap(bits_len + (size_t)(n - computed_up_to));

        char bin_pat[64];
        size_t pat_len;
        for (idx_t k = computed_up_to + 1; k <= n; ++k) {
            get_bin_pattern(k, bin_pat, &pat_len);

            /* search the pattern in the existing bit string */
            bool found = false;
            for (size_t i = 0; i + pat_len <= bits_len; ++i) {
                size_t j;
                for (j = 0; j < pat_len; ++j) {
                    if (bits[i + j] != (uint8_t)bin_pat[j]) break;
                }
                if (j == pat_len) { found = true; break; }
            }

            seq_t val = found ? (seq_t)1 : (seq_t)0;
            cache[k] = val;
            append_bit(val ? '1' : '0');
        }
        computed_up_to = n;
    }

    return cache[n];
}
