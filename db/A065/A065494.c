/*
 * A065494  Number of (binary) bit strings in which no even length block of
 *          0's is followed by an even length block of 1's.
 * https://oeis.org/A065494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A065494
   Number of binary strings of length n in which no even‑length block of 0's
   is followed by an even‑length block of 1's.
   Recurrence (from the generating function (1+x)^2/(1-3x^2-2x^3+x^4)):
     a(0)=1, a(1)=2, a(2)=4, a(3)=8
     a(n) = 3*a(n-2) + 2*a(n-3) - a(n-4)   for n ≥ 4
   The implementation memoizes computed values in a static array, growing
   it as needed.  All arithmetic is performed in the build's seq_t type,
   so the code works for arbitrary‑precision and fixed‑width builds alike.
*/

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache that grows to the required size */
    static seq_t *cache = NULL;
    static idx_t   len   = 0;   /* number of elements currently stored   */
    static idx_t   cap   = 0;   /* allocated capacity                     */

    /* enlarge cache if necessary */
    if (n >= len)
    {
        idx_t needed = n + 1;
        if (cap < needed)          /* grow, e.g. double each time */
        {
            idx_t newCap = cap ? cap * 2 : 8;
            if (newCap < needed) newCap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
            cache = tmp;
            cap   = newCap;
        }

        /* compute values up to index n */
        for (idx_t i = len; i <= n; ++i)
        {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)2;
            else if (i == 2) cache[i] = (seq_t)4;
            else if (i == 3) cache[i] = (seq_t)8;
            else
            {
                /* a(i) = 3*a(i-2) + 2*a(i-3) - a(i-4) */
                seq_t val = (seq_t)3 * cache[i-2];
                val += (seq_t)2 * cache[i-3];
                val -= cache[i-4];
                cache[i] = val;
            }
        }
        len = n + 1;   /* updated size of valid cache */
    }

    return cache[n];
}
