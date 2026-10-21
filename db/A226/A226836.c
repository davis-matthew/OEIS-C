/*
 * A226836  Squares s such that first m and last m digits of the binary
 *          representation are perfect positive squares written in binary, and
 *          m = floor(binaryLength(s)/2), where binaryLength(s) = A070939(s)
 *          is the binary length of s.
 * https://oeis.org/A226836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   bitlen_seq_t, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226836: squares s such that the first
   ⌊L/2⌋ and last ⌊L/2⌋ bits of the binary representation of s
   are perfect positive squares, where L = bitlen_seq_t(s). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/floor_sqrt.h"

/* Test whether an unsigned integer square satisfies the OEIS condition. */
static bool is_valid_sq(seq_t s)
{
    if (s == 0) return false;

    unsigned L = bitlen_seq_t(s);            /* binary length of s */
    unsigned m = L / 2;                      /* floor(L/2) */
    if (m == 0) return false;                /* need non‑empty halves */

    seq_t low  = s & (((seq_t)1 << m) - 1);  /* last m bits */
    seq_t high = s >> (L - m);               /* first m bits */

    if (low == 0 || high == 0) return false; /* must be positive squares */

    seq_t r_low  = floor_sqrt(low);
    if (r_low * r_low != low) return false;
    seq_t r_high = floor_sqrt(high);
    if (r_high * r_high != high) return false;

    return true;
}

/* Static cache for sequence terms. */
static seq_t *cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t last_root   = 0;

/* Grow the cache to at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    while (cache_len < (size_t)n) {
        ++last_root;
        seq_t sq = ((seq_t)last_root) * last_root;
        if (is_valid_sq(sq)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = sq;
        }
    }
}

/* OEIS sequence accessor. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, placeholder */
    ensure_cache(n);
    return cache[n-1];
}
