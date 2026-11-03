/*
 * A037386  Every base 3 digit of n is a base 10 digit of n.
 * https://oeis.org/A037386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3338, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 8200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth positive integer whose base‑3 digits are
   all present as decimal digits of the same integer. */

/* The function scans successive integers, checks the requirement
   (via _good) and memoises found terms so that calls with arbitrary
   n are fast.  The cache grows dynamically; the code works for any
   integer type seq_t supported by the build (fixed‑size or big‑int). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true iff every base‑3 digit of x appears in its decimal representation */
static inline bool _good(seq_t x)
{
    bool dec[10] = {0};
    bool tern[3] = {0};
    seq_t tmp = x;

    /* collect decimal digits */
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);
        dec[d] = true;
        tmp /= (seq_t)10;
    }

    /* collect base‑3 digits */
    tmp = x;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)3);
        tern[d] = true;
        tmp /= (seq_t)3;
    }

    /* every ternary digit must also be a decimal digit */
    for (int d = 0; d < 3; ++d)
        if (tern[d] && !dec[d])
            return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)      /* sequence is offset 1; return 0 for n==0 */
        return (seq_t)0;

    static seq_t *cache = NULL;     /* cached sequence values */
    static idx_t cache_count = 0;   /* number of values stored */
    static idx_t cache_cap   = 0;   /* capacity of cache array */
    static seq_t next_candidate = 1;/* next integer to test */

    while (cache_count < n) {
        if (_good(next_candidate)) {
            if (cache_count == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();      /* out of memory */
                cache = tmp;
                cache_cap = (idx_t)new_cap;
            }
            cache[cache_count++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
