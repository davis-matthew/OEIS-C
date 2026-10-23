/*
 * A042633  Denominators of continued fraction convergents to sqrt(846).
 * https://oeis.org/A042633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 4286590*a(n-16) - a(n-32) for n>31 (n>=0) a(0)=1,
 *            a(1)=11, a(2)=12, a(3)=23, a(4)=35, a(5)=58, a(6)=93, a(7)=151,
 *            a(8)=395, a(9)=546, a(10)=941, a(11)=1487, a(12)=2428,
 *            a(13)=3915, a(14)=6343, a(15)=73688, a(16)=4280247,
 *            a(17)=47156405, a(18)=51436652, a(19)=98593057, a(20)=150029709,
 *            a(21...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the denominators of the convergents
   to sqrt(846) by iterating the recurrence
      q_k = a_k * q_{k-1} + q_{k-2}
   where the continued‑fraction digits of sqrt(846) are
      a_0 = 29
      a_1..a_16 = 11,1,1,1,1,1,1,2,1,1,1,1,1,1,11,58
   and they repeat with period 16 thereafter.  The result is memoised
   so that calls with arbitrary (not necessarily increasing) n are O(n). */

#include <stdlib.h>

static const seq_t period[16] = {
    (seq_t)11, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
    (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
    (seq_t)1, (seq_t)1, (seq_t)11, (seq_t)58
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;        /* allocated size */
    static size_t cache_len = 0;        /* number of elements currently stored (next free index) */

    /* Ensure enough space for n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* OOM – caller will handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)11;
        } else {
            seq_t a_k = period[(i - 1) % 16];
            cache[i] = a_k * cache[i - 1] + cache[i - 2];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
