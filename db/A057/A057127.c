/*
 * A057127  -2 is a square mod n.
 * https://oeis.org/A057127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 989 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A057127: numbers n such that −2 is a quadratic residue modulo n.
    The sequence consists of all positive integers that are not divisible
    by 4 and whose odd prime factors are congruent to 1 or 3 (mod 8).
    For each odd prime p, the Legendre symbol (−2|p)=1 exactly when p≡1,3
    (mod 8).  The same holds for prime powers by Hensel lifting, and the
    factor 2 may occur at most once (powers ≥4 fail modulo 4).             */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a given positive integer x satisfies the condition for
   A057127.  x is given as idx_t (uint64_t).                     */
static bool isGood(idx_t x)
{
    if (x % 4 == 0)                /* multiples of 4 are excluded            */
        return false;

    idx_t t = x;
    if (t % 2 == 0)                /* strip the single factor 2, if present */
        t /= 2;

    /* trial divide by odd numbers up to sqrt(t).  Use division
       to avoid overflow when computing p*p.                     */
    for (idx_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            /* p must be ≡1 or 3 (mod 8)                              */
            if ((p % 8 != 1) && (p % 8 != 3))
                return false;
            while (t % p == 0)
                t /= p;
        }
    }

    /* any remaining prime factor larger than sqrt(original) */
    if (t > 1) {
        if ((t % 8 != 1) && (t % 8 != 3))
            return false;
    }
    return true;
}

/* Return the n‑th term of A057127 (1‑based index).  The function
   keeps a static cache so that calls with arbitrary n perform in
   O(n) time overall.                                                      */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of found terms                */
    static idx_t cache_len = 0;   /* number of elements stored           */
    static idx_t cache_cap = 0;   /* allocated capacity                  */
    static idx_t cur = 1;         /* next integer to test                */

    /* grow the cache until we have at least n terms */
    while (cache_len < n) {
        if (isGood(cur)) {
            if (cache_len == cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_cap);
                if (!tmp)                     /* allocation failed */
                    return 0;                /* caller should not receive data */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur;     /* store the good number   */
        }
        ++cur;                                 /* proceed to next integer */
    }

    return cache[n - 1];
}
