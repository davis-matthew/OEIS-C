/*
 * A060979  |First digit - second digit + third digit - fourth digit ...| =
 *          11.
 * https://oeis.org/A060979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A060979 – numbers whose alternating digit sum (in absolute value)
   equals 11 (i.e. a - b + c - d + ... = ±11).  The function returns the
   n-th such number, with a(1) = 209.  The routine guarantees that any
   value that fits in seq_t is returned exactly. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"

/* Compute the absolute value of the alternating sum of the digits of a
   non‑negative integer in decimal (most significant digit has + sign). */
static int alt_sum_abs(uint64_t x)
{
    int sign = +1;
    int diff = 0;
    uint8_t buf[32];
    size_t len = 0;

    /* Extract digits (least‑significant first). */
    do {
        buf[len++] = (uint8_t)(x % 10);
        x /= 10;
    } while (x != 0);

    /* Process digits from most‑significant to least‑significant. */
    for (size_t i = 0; i < len; ++i) {
        diff += sign * (int)buf[len - 1 - i];
        sign = -sign;
    }
    return diff < 0 ? -diff : diff;
}

/* Static cache of computed terms.  cache_size contains the number of terms
   actually stored; cache_cap is the allocated capacity. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;   /* number of terms already in the cache */

/* The next candidate to test for membership in the sequence.  The search
   starts at the smallest three‑digit number. */
static idx_t next_candidate = 100; /* 100 is the smallest 3‑digit integer */

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n < 1 – behaviour does not matter */

    /* Ensure that the underlying array can hold at least n elements. */
    if (cache_size < (size_t)n) {
        /* Grow or create the cache.  The helper handles construction
           of seq_t elements and any required copying for user defined
           big‑integer types. */
        ensure_cache((idx_t)n, &cache, &cache_cap);

        /* Generate terms until the cache contains n of them. */
        while (cache_size < (size_t)n) {
            if (alt_sum_abs((uint64_t)next_candidate) == 11) {
                cache[cache_size] = (seq_t)next_candidate;
                ++cache_size;
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
