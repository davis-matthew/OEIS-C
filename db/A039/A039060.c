/*
 * A039060  Numbers whose base-8 representation has the same number of 1's and
 *          6's.
 * https://oeis.org/A039060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑8 representation has equal number of 1's and
 *            6's
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether a number has the same number of 1’s and 6’s in octal */
static bool equal_ones_sixes(seq_t v)
{
    if (v == (seq_t)0)
        return true;          /* 0 has 0 of each */
    int ones = 0, sixes = 0;
    while (v > (seq_t)0) {
        int digit = (int)(v % (seq_t)8);
        if (digit == 1)
            ++ones;
        else if (digit == 6)
            ++sixes;
        v = v / (seq_t)8;
    }
    return ones == sixes;
}

/* Return the n-th term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */
    static seq_t next_candidate = (seq_t)0; /* next number to test */

    /* initialize state on first call */
    if (cache_len == 0) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        /* term a(1) = 0 */
        cache[0] = (seq_t)0;
        cache_len = 1;
        next_candidate = (seq_t)1;
    }

    /* generate terms until we have the desired index */
    while (cache_len < n) {
        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }

        /* test next_candidate */
        if (equal_ones_sixes(next_candidate)) {
            cache[cache_len++] = next_candidate;
        }
        /* advance */
        next_candidate = next_candidate + (seq_t)1;
    }

    /* return the n-th term (1‑based) */
    return cache[n - 1];
}
