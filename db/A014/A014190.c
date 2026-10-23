/*
 * A014190  Palindromes in base 3 (written in base 10).
 * https://oeis.org/A014190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A014190: the n‑th palindrome in base 3, ordered by decimal value. */
/* Implements a(1) = 0, a(2) = 1, a(3) = 2, … (see OEIS). */
/* Uses a cache of previously generated palindromes; O(n) time, O(n) space. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* External types supplied by the build. */
extern seq_t a(idx_t n);

/* Static state for caching palindromes. */
static seq_t *pal_cache = NULL;   /* stored palindromes */
static size_t pal_cap   = 0;      /* allocated capacity */
static size_t pal_len   = 0;      /* number of stored palindromes */
static idx_t  gen_len   = 0;      /* length of the last generated palindrome */

/* Compute 3^k for a non‑negative integer k. */
static seq_t pow3(idx_t k)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        r *= (seq_t)3;
    return r;
}

/* Ensure the cache has room for at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (pal_cache) {
        memcpy(tmp, pal_cache, pal_len * sizeof(seq_t));
        free(pal_cache);
    }
    pal_cache = tmp;
    pal_cap   = new_cap;
}

/* Append a palindrome to the cache, growing the array if necessary. */
static void add_pal(seq_t v)
{
    if (pal_len == pal_cap) {
        size_t new_cap = pal_cap ? pal_cap << 1 : 128;
        grow_cache(new_cap);
    }
    pal_cache[pal_len++] = v;
}

/* Generate all base–3 palindromes of length L (L ≥ 1) and add them to
   the cache.  Length refers to the number of base–3 digits. */
static void generate_len(idx_t L)
{
    idx_t k = (L + 1) / 2;          /* half‑length (rounded up)   */
    seq_t start = (L == 1) ? (seq_t)0 : pow3(k - 1);   /* first half cannot start with 0 unless L == 1 */
    seq_t end   = pow3(k);                               /* exclusive upper bound for the first half */

    for (seq_t h = start; h < end; ++h) {
        /* Extract the k digits of the first half, most significant first. */
        uint8_t dig[64];   /* 64 digits is far more than needed for k <= 40 */
        seq_t h_local = h;
        for (idx_t i = 0; i < k; ++i) {
            dig[k - 1 - i] = (uint8_t)(h_local % (seq_t)3);
            h_local /= (seq_t)3;
        }

        /* Build the full palindrome. */
        seq_t val = (seq_t)0;
        for (idx_t i = 0; i < k; ++i)
            val = val * (seq_t)3 + (seq_t)dig[i];

        if (L & 1) {  /* odd length – skip the middle digit on mirroring */
            for (int dec = (int)k - 2; dec >= 0; --dec)
                val = val * (seq_t)3 + (seq_t)dig[dec];
        } else {      /* even length – mirror all k digits */
            for (int dec = (int)k - 1; dec >= 0; --dec)
                val = val * (seq_t)3 + (seq_t)dig[dec];
        }

        add_pal(val);
    }
}

/* Ensure that at least n palindromes are stored in the cache. */
static void ensure_n(idx_t n)
{
    while (pal_len < n) {
        ++gen_len;
        generate_len(gen_len);
    }
}

/* Return the n-th base‑3 palindrome in decimal representation. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out‑of‑offset value; harmless */
    ensure_n(n);
    return pal_cache[n - 1];
}
