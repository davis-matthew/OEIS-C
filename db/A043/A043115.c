/*
 * A043115  Numbers k such that 3 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043115: Numbers k such that the digits 3 and 4 occur
   adjacent in the base‑5 representation of k, but not in the representation
   of k−1.  The implementation searches sequentially and caches the
   found terms. */
#include <stdlib.h>
#include <stdbool.h>

/* Returns true if base‑5 representation of x contains the pair 34 or 43 */
static bool has34(seq_t x)
{
    if (x == 0) return false;
    seq_t prev = x % 5;      /* least significant digit */
    seq_t t    = x / 5;
    while (t != 0) {
        seq_t d = t % 5;     /* next more significant digit */
        if ((prev == 3 && d == 4) || (prev == 4 && d == 3))
            return true;
        prev = d;
        t   /= 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Lazy static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t next_k = 0;   /* candidate counter */

    /* Find enough terms to cover n */
    while (cache_size < (size_t)n) {
        /* Advance to next candidate */
        next_k = (next_k == 0) ? 1 : next_k + 1;

        seq_t k   = next_k;
        seq_t km1 = k - 1;

        if (has34(k) && !has34(km1)) {
            /* Ensure capacity */
            if (cache_size >= cache_cap) {
                size_t newcap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_size++] = k;
        }
    }

    return cache[n - 1];
}
