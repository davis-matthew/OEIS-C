/*
 * A044220  Numbers k such that string 4,1 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers k such that the base‑8 representation of k contains the
 *            substring "41" but the base‑8 representation of k-1 does not
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Check if a non‑negative integer x (in base 8) contains the digit pair 4,1. */
static bool has41(seq_t x)
{
    if (x == 0)                     /* 0 has no digits "41" */
        return false;

    /* 128‑bit numbers need at most 43 octal digits; 64 digits is safe. */
    uint8_t digits[64];
    size_t len = 0;
    seq_t t = x;

    while (t > 0) {
        digits[len++] = (uint8_t)(t % (seq_t)8);   /* least‑significant first */
        t /= (seq_t)8;
    }

    /* Scan most‑significant to least‑significant for the pair 4,1. */
    for (int i = (int)len - 1; i > 0; --i) {
        if (digits[i] == 4 && digits[i - 1] == 1)
            return true;
    }
    return false;
}

/* Static cache of already found terms. */
static seq_t *cache          = NULL;
static size_t cache_size     = 0;
static size_t cache_cap      = 0;   /* capacity of the cache */
static idx_t   last_checked  = 0;   /* largest k examined so far */

/* Computes the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)               /* behaviour for n < 1 is undefined; return 0 */
        return 0;

    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
        last_checked = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Check next integer k. */
        ++last_checked;
        seq_t k = (seq_t)last_checked;

        if (has41(k) && !has41(k - (seq_t)1)) {
            /* Add to cache. */
            if (cache_size + 1 > cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = k;
        }
    }

    return cache[n - 1];
}
