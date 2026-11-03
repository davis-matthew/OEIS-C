/*
 * A051805  Numbers with nonzero multiplicative digital root 3.
 * https://oeis.org/A051805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051805: Numbers with nonzero multiplicative digital root 3.
   The function a(n) returns the nth term of the sequence (n>=1).
   The multiplicative digital root of a number is defined by repeatedly
   multiplying all of its non‑zero digits until a single digit is obtained.
   This implementation keeps a growing cache of discovered terms and
   extends it on demand until the requested index is present. */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t md_root(seq_t n)
{
    if (n == 0)
        return 0;
    seq_t res;
    /* repeat until a single digit is left */
    while (n >= 10) {
        res = 1;
        while (n > 0) {
            seq_t d = n % 10;
            if (d != 0)
                res *= d;
            n /= 10;
        }
        n = res;
    }
    return n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for n==0; return 0 */

    /* static cache of found terms */
    static seq_t *cache = (seq_t *)0;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t next_num = 1;   /* next integer to test */

    /* extend the cache until it contains index n */
    while (cache_len < n) {
        if (md_root(next_num) == 3) {
            if (cache_len == cache_cap) {   /* grow capacity */
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = next_num;
        }
        next_num++;
    }

    return cache[n - 1];
}
