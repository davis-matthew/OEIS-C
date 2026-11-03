/*
 * A044229  Numbers k such that string 5,2 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 8644 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = the nth integer k such that the octal representation of k
   contains the substring "52", while the octal representation of k-1 does not.
   The implementation performs an incremental search and memoises found terms. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* return true iff the octal representation of x contains the consecutive
   digits 5 followed by 2.  Pattern '52' is detected as a higher-order 5
   followed by a lower-order 2 while scanning least‑significant digit first. */
static bool has52_oct(seq_t x)
{
    const seq_t e8 = (seq_t)8;
    const seq_t two = (seq_t)2;
    const seq_t five = (seq_t)5;

    if (x < (seq_t)2)          /* smallest possible value with "52" is 0o52 = 42 */
        return false;

    seq_t prev = x % e8;
    x = x / e8;
    while (x != (seq_t)0) {
        seq_t curdig = x % e8;
        if (prev == two && curdig == five)
            return true;
        prev = curdig;
        x = x / e8;
    }
    return false;
}

static seq_t *cache = NULL;      /* memoised sequence values          */
static size_t cache_len = 0;      /* number of values stored           */
static size_t cache_cap = 0;      /* allocated capacity of cache       */
static seq_t cur = (seq_t)0;      /* next integer to test              */
static bool prev_has = false;     /* whether previous integer had "52" */

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence offset is 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
    }

    while (cache_len < (size_t)n) {
        bool h = has52_oct(cur);
        if (h && !prev_has) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = cur;
        }
        prev_has = h;
        cur++;
    }

    return cache[(size_t)(n - 1)];
}
