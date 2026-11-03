/*
 * A039052  Numbers whose base-8 representation has the same number of 0's and
 *          3's.
 * https://oeis.org/A039052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 7741 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches the positive integers and keeps those
   whose base‑8 representation contains the same number of 0’s and 3’s.
   The sequence is computed by brute‑force search; no closed formula
   exists.  The function memoises results so that subsequent calls in
   any order are fast. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline bool has_equal_zeros_threes(seq_t x)
{
    unsigned int count0 = 0, count3 = 0;
    while (x != 0) {
        unsigned int d = (unsigned int)(x % 8);
        if (d == 0)      count0++;
        else if (d == 3) count3++;
        x /= 8;
    }
    return count0 == count3;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of found values                */
    static idx_t   size   = 0;     /* number of elements stored in cache   */
    static seq_t   next   = 1;     /* next candidate to test               */

    if (n == 0) return 0;          /* offset is 1; n>=1 in practice       */

    if (n <= size) return cache[n - 1];

    /* Grow the cache to hold at least n values */
    seq_t *tmp = mem_resize(seq_t, cache, (size_t)n);
    if (!tmp) {
        /* Allocation failure: return 0 as a fallback (unlikely). */
        return 0;
    }
    cache = tmp;

    /* Scan numbers starting from 'next' until we have stored n values */
    seq_t cand = next;
    while (size < n) {
        if (has_equal_zeros_threes(cand)) {
            cache[size] = cand;
            size++;
        }
        cand++;
    }
    next = cand;           /* next search will start from cand    */
    return cache[n - 1];
}
