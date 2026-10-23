/*
 * A162558  a(n) = ((3+sqrt(3))*(5+sqrt(3))^n + (3-sqrt(3))*(5-sqrt(3))^n)/6.
 * https://oeis.org/A162558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A162558
   Recurrence:  a(0)=1, a(1)=6,
               a(n)=10*a(n-1)-22*a(n-2)   for n>1
   This function caches results in a static array so that repeated calls
   are fast and order‑independent.  The cache is resized by doubling
   capacity when needed.  The implementation uses only integer
   arithmetic and does not depend on any external utility functions.  */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache buffer */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity */
    static size_t  len   = 0;   /* number of terms stored (== len is next free index) */

    /* initialise cache with first two terms if needed */
    if (cache == NULL) {
        cap = 2;
        len = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;               /* a(0) */
        cache[1] = (seq_t)6;               /* a(1) */
    }

    /* extend cache up to n if required */
    while (len <= (size_t)n) {
        /* compute next term using the recurrence */
        seq_t next = (seq_t)(10 * cache[len - 1] - 22 * cache[len - 2]);

        /* grow buffer if capacity exceeded */
        if (len == cap) {
            cap <<= 1;  /* double capacity */
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        cache[len] = next;
        ++len;
    }

    return cache[(size_t)n];
}
