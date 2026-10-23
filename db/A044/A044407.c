/*
 * A044407  Numbers n such that string 7,5 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n whose decimal representation contains the
   substring "75" but (n−1) does not. Computation is done by linear
   search with memoisation; the cached values are stored in a dynamic
   array and expanded as required. */

/* We can safely use the standard headers; no C++ specific
   constructs are needed. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Check whether a decimal number contains the substring 75.
   The number is processed from least‑significant digit to most
   significant digit; the test is therefore that a digit 7 followed
   immediately by a digit 5 is found. */
static inline bool has_75(seq_t x)
{
    /* Less than two digits → cannot contain 75. */
    if (x < (seq_t)10) {
        return false;
    }
    seq_t tmp = x;
    int prev = -1;               /* previous lower‑order digit */
    while (tmp != 0) {
        int digit = (int)(tmp % 10);
        if (digit == 7 && prev == 5) { /* pattern 75 found */
            return true;
        }
        prev = digit;
        tmp /= 10;
    }
    return false;
}

/* Static cache for first‑n terms that have already been found. */
static seq_t *cache = NULL;
static size_t cached = 0;       /* number of items stored */
static size_t cap    = 0;       /* capacity of 'cache' array */
static seq_t next_check = (seq_t)75;   /* next integer to test */

/* Return the n‑th term of the sequence.  a(1)=75. */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (!cache) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* Generate more terms until we have at least n of them. */
    while (cached < (size_t)n) {
        if (has_75(next_check) && !has_75(next_check - 1)) {
            /* Grow the cache if necessary. */
            if (cached == cap) {
                size_t new_cap = cap * 2;
                seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache = new_cache;
                cap = new_cap;
            }
            cache[cached++] = next_check;
        }
        ++next_check;
    }

    return cache[n - 1];
}
