/*
 * A197311  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 0,1,1,0,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3) - 9*a(n-4) + 10*a(n-5) -
 *            3*a(n-7) with initial values a(1)=1, a(2)=0, a(3)=3, a(4)=2,
 *            a(5)=9, a(6)=6, a(7)=27
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 77, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;        /* 1‑based cache of previously computed terms */
static idx_t a_cache_size = 0;       /* one past the largest index stored */

/* Compute the n-th term of A197311.  The function accepts and returns
   values in the seq_t type, which may be an arbitrary‑precision integer
   or an unsigned integer type.  The recurrence uses only addition,
   subtraction and multiplication by small constants, so it works for
   all supported seq_t types. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* out of range, but not used */

    /* Grow cache if necessary */
    if (n >= a_cache_size)
    {
        idx_t newsize = n + 1;    /* include index 0 */
        seq_t *tmp = (seq_t *)realloc(a_cache,
                                      (size_t)newsize * sizeof(seq_t));
        if (!tmp)                 /* out of memory – abort gracefully */
            return 0;
        a_cache = tmp;
        a_cache_size = newsize;

        /* Initialise base values (they are the same for every call). */
        if (a_cache_size >= 2) a_cache[1] = (seq_t)1;
        if (a_cache_size >= 3) a_cache[2] = (seq_t)0;
        if (a_cache_size >= 4) a_cache[3] = (seq_t)3;
        if (a_cache_size >= 5) a_cache[4] = (seq_t)2;
        if (a_cache_size >= 6) a_cache[5] = (seq_t)9;
        if (a_cache_size >= 7) a_cache[6] = (seq_t)6;
        if (a_cache_size >= 8) a_cache[7] = (seq_t)27;

        /* Compute new terms using the empirical recurrence.  The
           computation uses only positive intermediate values, so
           unsigned subtraction is safe. */
        for (idx_t i = 8; i <= n; ++i)
        {
            seq_t pos = a_cache[i-1];
            pos += (seq_t)6 * a_cache[i-2];
            pos += (seq_t)10 * a_cache[i-5];

            seq_t sub = (seq_t)6 * a_cache[i-3];
            sub += (seq_t)9 * a_cache[i-4];
            sub += (seq_t)3 * a_cache[i-7];

            a_cache[i] = pos - sub;   /* pos >= sub by definition of the sequence */
        }
    }
    return a_cache[n];
}
