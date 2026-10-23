/*
 * A039030  Numbers whose base-7 representation has the same number of 0's and
 *          1's.
 * https://oeis.org/A039030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer whose base-7 representation
   has an equal number of 0's and 1's.  The function scans numbers
   in increasing order, maintaining a cache of already-found terms.
   It works for any seq_t type (unsigned integer of 8/16/32/64/128 bits
   or an arbitrary‑precision class). */
#include <stdlib.h>     /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next number to test */
    static seq_t curr = 1;

    if (n == 0)
        return 0;  /* undefined for n<1, but return 0 to avoid UB */

    /* generate terms until we have at least n in the cache */
    while (cache_len < n)
    {
        seq_t x   = curr;
        seq_t tmp = x;
        int  count0 = 0, count1 = 0;

        /* count 0's and 1's in the base‑7 representation */
        while (tmp != 0)
        {
            int digit = (int)(tmp % (seq_t)7);
            if (digit == 0)   count0++;
            else if (digit == 1) count1++;
            tmp = tmp / (seq_t)7;
        }

        /* for numbers <7, tmp becomes 0 immediately, so count0=count1=0
           which correctly counts as a valid term (e.g., 2..6).               */

        if (count0 == count1)
        {
            /* grow cache if necessary */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }

        curr++;   /* move to next candidate */
    }

    return cache[n - 1];
}
