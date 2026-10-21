/*
 * A061362  Numbers in which each nonzero digit divides the number formed by
 *          the rest, i.e., the number obtained by just removing this digit.
 * https://oeis.org/A061362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A061362: Numbers in which each non‑zero digit divides the number formed by the
   remaining digits.  The function computes the n‑th term by brute‑force
   search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a number satisfies the definition. */
static bool
good(seq_t x)
{
    if (x == 0)                /* 0 has no non‑zero digits */
        return true;

    seq_t orig   = x;
    seq_t pow10  = 1;          /* current power of 10 (10^i) */
    seq_t ten    = 10;         /* constant 10 */

    for (seq_t tmp = orig; tmp > 0; tmp /= 10, pow10 *= 10) {
        int digit = (int)(tmp % 10);   /* digit is in [0,9] */

        if (digit == 0)
            continue;

        seq_t high = orig / (pow10 * ten);  /* digits above position i */
        seq_t low  = orig % pow10;           /* digits below position i */
        seq_t rest = high * pow10 + low;

        if ((rest % digit) != 0)
            return false;
    }
    return true;
}

/* Cache to store found terms. */
static seq_t *cache      = NULL;
static idx_t cache_cap   = 0;
static idx_t cache_len   = 0;
static seq_t next_candidate = 0;      /* number to test next */

/* Append a new value to the cache, enlarging it if necessary. */
static void
cache_append(seq_t v)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 1024;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = v;
}

/* Sequence entry function. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n <= 0, return 0 for safety */

    while (cache_len < n) {
        /* Find the next good number. */
        for (;;) {
            if (good(next_candidate)) {
                cache_append(next_candidate);
                next_candidate++;   /* advance to the next candidate */
                break;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
