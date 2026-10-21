/*
 * A203803  G.f.: exp( Sum_{n>=1} A000204(n)^3 * x^n/n ) where A000204 is the
 *          Lucas numbers.
 * https://oeis.org/A203803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A203803
   Generating function:  A(x) = 1 / ((1 + x - x^2)^3 * (1 - 4x - x^2))
   The recurrence (for n ≥ 8) is
       a(n) = a(n‑1) + 13·a(n‑2) + 8·a(n‑3)
              – 20·a(n‑4) – 8·a(n‑5) + 13·a(n‑6)
              – a(n‑7) – a(n‑8)
   with initial values
       a(0)=1, a(1)=1, a(2)=14, a(3)=35,
       a(4)=205, a(5)=744, a(6)=3414, a(7)=13926.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* keep a cache of all computed terms up to the maximum n seen */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;          /* allocated size */
    static idx_t   up_to = 0;          /* first index not yet computed  */

    /* enlarge the array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap > 0 ? cap : 8;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the first eight terms on the first call (or after
       the cache was (re)allocated to a very small size). */
    if (up_to == 0) {
        cache[0] = (seq_t)1;   cache[1] = (seq_t)1;
        cache[2] = (seq_t)14;  cache[3] = (seq_t)35;
        cache[4] = (seq_t)205; cache[5] = (seq_t)744;
        cache[6] = (seq_t)3414;cache[7] = (seq_t)13926;
        up_to = 8;
    }

    /* compute any missing terms up to n */
    for (idx_t i = up_to; i <= n; ++i) {
        if (i < 8) continue;         /* already initialised */

        /* use the recurrence */
        seq_t val = cache[i-1];
        val += (seq_t)13 * cache[i-2];
        val += (seq_t)8  * cache[i-3];
        val -= (seq_t)20 * cache[i-4];
        val -= (seq_t)8  * cache[i-5];
        val += (seq_t)13 * cache[i-6];
        val -= cache[i-7];
        val -= cache[i-8];
        cache[i] = val;
    }

    up_to = n + 1;
    return cache[n];
}
