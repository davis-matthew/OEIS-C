/*
 * A039107  Numbers whose base-9 representation has the same number of 5's and
 *          7's.
 * https://oeis.org/A039107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_fives_and_sevens, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A039107  oeis.org
 *
 *  Sequence:  Numbers whose base‑9 representation has the same number of 5’s
 *  and 7’s.  a(1)=0, a(2)=1, a(3)=2, …
 *
 *  The function computes the n-th term (1‑based) by generating numbers
 *  in ascending order and keeping a cache of all terms found so far.
 *  Each candidate number is tested by converting it to base‑9 and
 *  counting the digits 5 and 7.  Once enough terms have been found
 *  the cached n‑th value is returned.  The algorithm runs in
 *  O(n) time for typical n (≤10⁴) and requires only O(n) memory.
 *
 *  All arithmetic uses the seq_t type; the helper seq_realloc is
 *  employed so the code works with both built‑in integer types
 *  (C) and arbitrary‑precision integer classes (C++).
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"
#include "../utils/has_equal_fives_and_sevens.h"

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;

seq_t a(idx_t n)
{
    /*  Ensure the cache contains at least n terms.  */
    while (cache_len < n) {
        seq_t cand = last_checked++;
        if (has_equal_fives_and_sevens(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }
    return cache[n - 1];
}
