/*
 * A061880  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 11.
 * https://oeis.org/A061880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 991 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061880:  a(n) is the nth positive integer whose alternating
   digit sum (first digit positive, second negative, third positive, ...) equals 11.
   The function incrementally searches for such numbers and memoises the
   results so that repeated calls are fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute alternating digit sum (sign + for MSB, then - for next digit,
   etc.) and test whether it equals 11.  The calculation is performed
   using only integer arithmetic. */
static bool alt_sum_is_11(seq_t x) {
    unsigned char digits[64];        /* Enough for any seq_t that fits in 64 bits */
    size_t ndig = 0;

    if (x == (seq_t)0) {
        return false;                /* 0 does not satisfy the condition */
    }

    /* Extract digits in little‑endian order. */
    while (x != (seq_t)0) {
        digits[ndig++] = (unsigned char)(x % (seq_t)10);
        x /= (seq_t)10;
    }

    /* Accumulate from the most significant digit. */
    bool positive = true;
    int sum = 0;
    for (size_t i = 0; i < ndig; ++i) {
        unsigned char d = digits[ndig - 1 - i];
        sum += positive ? d : -d;
        positive = !positive;
    }

    return sum == 11;
}

/* Return the nth term of the sequence (1‑based index).  The implementation
   keeps a cache of found terms and reuses it for subsequent calls. */
seq_t a(idx_t n)
{
    /* Static state that persists between calls. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  found      = 0;      /* number of terms currently cached */
    static seq_t  candidate  = 1;      /* next integer to test */

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        /* If allocation fails we abort; the test harness will not continue. */
        if (cache == NULL) abort();
    }

    /* Expand the cache until we have the n‑th term. */
    while (found < n) {
        if (alt_sum_is_11(candidate)) {
            /* Store the new term. */
            if (found == (idx_t)cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[found++] = candidate;
        }
        ++candidate;
    }

    return cache[n - 1];
}
