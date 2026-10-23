/*
 * A021229  Expansion of 1/((1-x)(1-2x)(1-7x)(1-9x)).
 * https://oeis.org/A021229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A021229 – Expansion of 1/((1-x)(1-2x)(1-7x)(1-9x))
   Formula: a(n) = 19*a(n-1) - 113*a(n-2) + 221*a(n-3) - 126*a(n-4)   (n>3)
   Initial values:
      a(0) = 1
      a(1) = 19
      a(2) = 248
      a(3) = 2786
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cached sequence values          */
    static size_t cap   = 0;             /* current capacity of the cache   */
    static idx_t last   = (idx_t)-1;     /* last computed index, sentinel   */

    /* Ensure cache can hold at least n+1 entries */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = cap ? cap * 2 : 4;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;                     /* realloc with NULL is allowed   */
        cap   = newcap;
    }

    /* One‑time initialisation of the first four terms */
    if (last == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)248;
        cache[3] = (seq_t)2786;
        last = 3;
    }

    /* Compute any required terms up to n */
    while (last < n) {
        idx_t i = last + 1;
        seq_t next =
            (seq_t)19   * cache[last] -
            (seq_t)113  * cache[last - 1] +
            (seq_t)221  * cache[last - 2] -
            (seq_t)126  * cache[last - 3];
        cache[i] = next;
        last = i;
    }

    return cache[n];
}
