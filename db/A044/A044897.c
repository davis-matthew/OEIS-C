/*
 * A044897  Base 11 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence implementation: A044897
   Base 11 run lengths alternate: even, odd, even,...
   A(n) is the n-th positive integer whose base‑11 representation
   consists of runs of identical digits whose lengths alternate in
   parity, starting with an even length run.
   Example: 12 (base 11 = "11") has a run of length 2 (even),
   then 134 (base 11 = "112") has runs of lengths 2 (even) and 1 (odd). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool runs_alternate_even_first(seq_t x)
{
    if (x == 0) return false;           /* no 0 in the sequence */

    /* collect base‑11 digits (least significant first) */
    unsigned char digits[128];
    int len = 0;
    seq_t t = x;
    while (t > 0) {
        seq_t d = t % 11;                /* d < 11 always */
        t = t / 11;
        digits[len++] = (unsigned char)d;
    }

    /* traverse digits from most significant to least significant */
    int expectedParity = 0;  /* 0 = even, 1 = odd; first run must be even */
    seq_t runLen = 1;

    for (int i = len - 2; i >= 0; --i) {
        if (digits[i] == digits[i + 1]) {
            ++runLen;
        } else {
            if ((runLen & 1) != expectedParity) return false;
            expectedParity ^= 1;  /* toggle parity */
            runLen = 1;
        }
    }
    /* check last run */
    return ((runLen & 1) == expectedParity);
}

seq_t a(idx_t n)
{
    /* lazy memoization: keep computed values in a static array */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = 1;   /* next integer to test */

    if (n == 0) return 0;  /* offset is 1; behaviour below offset is undefined */

    /* grow cache as needed */
    while (cache_size < n) {
        /* find next integer that satisfies the property */
        while (!runs_alternate_even_first(candidate))
            ++candidate;

        /* add to cache */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = mem_new(seq_t, new_cap);
            if (!new_cache) exit(EXIT_FAILURE);  /* allocation failed */
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            mem_free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = candidate;
        ++candidate;   /* prepare next candidate for future calls */
    }

    return cache[n - 1];
}
