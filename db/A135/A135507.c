/*
 * A135507  a(1) = 1; for n > 1, a(n) = 2*a(n-1) + lcm(a(n-1),n).
 * https://oeis.org/A135507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 50,
 *            big any n
 * Helpers:   my_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(1) = 1;
 *              a(n) = 2*a(n-1) + lcm(a(n-1), n)  for n > 1
 * where   lcm(x,y) = (x / gcd(x,y)) * y
 */
 
#include <stdlib.h>
#include "../utils/my_gcd.h"
 
/* --------------------------------------------------------------------
 * Small Euclidean gcd for seq_t.  No external dependency is used.
 * -------------------------------------------------------------------- */

 
/* --------------------------------------------------------------------
 * Cached evaluation of a(n).  The cache is resized by doubling.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* out of bounds – return 0 safely */
        return 0;
 
    /* static cache: index 0 holds a(1) */
    static seq_t *cache   = NULL;
    static idx_t  len     = 0;   /* number of entries stored   */
    static idx_t  cap     = 0;   /* allocated capacity        */
 
    /* If cache is empty, create capacity for first element */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (!cache) exit(1);      /* allocation failure */
    }
 
    /* Extend cache until we have n entries (a(1)..a(n)) */
    if (n > len) {
        /* Ensure capacity */
        while (cap < n) {
            size_t new_cap = (size_t)cap * 2;
            if (new_cap == 0) new_cap = 1;  /* just in case */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cap   = (idx_t)new_cap;
        }
 
        /* Compute missing values */
        for (idx_t i = len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = 1;               /* a(1) = 1 */
            } else {
                seq_t prev   = cache[i - 2];          /* a(i-1) */
                seq_t i_seq  = (seq_t)i;              /* current index as seq_t */
                seq_t g      = my_gcd(prev, i_seq);   /* gcd(a(i-1), i) */
                seq_t lcm    = (prev / g) * i_seq;    /* lcm(a(i-1), i) */
                val = (prev << 1) + lcm;              /* 2*a(i-1) + lcm */
            }
            cache[i - 1] = val;
        }
        len = n;
    }
 
    return cache[n - 1];
}
