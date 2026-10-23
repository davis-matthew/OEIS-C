/*
 * A164564  A positive integer n is included if there are at least two
 *          distinct odd numerical values of substrings in the binary
 *          representation of n that are not coprime to each other.
 * https://oeis.org/A164564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164564
   A positive integer n is included if there are at least two distinct odd numerical
   values of substrings in the binary representation of n that are not coprime to
   each other.
   The function below generates the sequence by searching integers in
   increasing order, caching the results for arbitrary‑order calls. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Return true if x has two distinct odd binary substrings with a common
   divisor > 1. */
static bool satisfies(uint64_t x)
{
    if (x == 0) return false;

    /* number of bits in x */
    unsigned int L = 64 - __builtin_clzll(x);

    /* store distinct odd substring values */
    uint64_t odd_vals[64];
    size_t odd_cnt = 0;

    for (int start = (int)L - 1; start >= 0; --start) {
        uint64_t val = 0;
        for (int end = start; end >= 0; --end) {
            val = (val << 1) | ((x >> end) & 1ULL);
            if ((val & 1ULL) == 0) continue;                /* even values ignored */

            /* skip duplicates */
            bool dup = false;
            for (size_t k = 0; k < odd_cnt; ++k) {
                if (odd_vals[k] == val) { dup = true; break; }
            }
            if (dup) continue;

            /* compare new value with all previous distinct odds */
            for (size_t k = 0; k < odd_cnt; ++k) {
                seq_t g = gcd((seq_t)odd_vals[k], (seq_t)val);
                if (g > 1) return true;
            }
            odd_vals[odd_cnt++] = val;
        }
    }
    return false;
}

/* Cache of computed a(n) values. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Return the nth member of sequence A164564 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined but safe */

    /* extend cache until it contains n entries */
    while (cache_len < (size_t)n) {
        uint64_t candidate;
        if (cache_len == 0)
            candidate = 1;
        else
            candidate = (uint64_t)cache[cache_len - 1] + 1ULL;

        /* find next integer that satisfies the condition */
        while (!satisfies(candidate)) ++candidate;

        /* grow storage if needed */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)candidate;
    }
    return cache[n - 1];
}
