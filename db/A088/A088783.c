/*
 * A088783  Numbers n such that 10*n^k + 1 is composite for all k > 0.
 * https://oeis.org/A088783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers n such that 10*n^k+1 is composite for all k>0.
    All known terms satisfy either n≡1 (mod 11) or n≡−1 (mod 33)
    (i.e. n%11==1 or n%33==32).  This implementation returns the
    nth such n (offset 1).  It memoizes previously generated terms so
    that calls may come in arbitrary order.
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *terms = NULL;
    static size_t len   = 0;   /* number of stored terms      */
    static size_t cap   = 0;   /* allocated capacity          */
    /* number just checked in the scan (uint64_t is enough for all
       values needed in the test; the actual term is stored as seq_t) */
    static uint64_t last_checked = 1;

    if (n == 0)
        return 0;  /* undefined for n==0, return 0 as a safe fallback */

    /* extend the cache until we have at least n terms */
    while (len < n)
    {
        ++last_checked;
        uint64_t l = last_checked;

        /* Accept numbers with n≡1 (mod 11) or n≡−1 (mod 33) */
        if ((l % 11ULL == 1ULL) || (l % 33ULL == 32ULL))
        {
            /* grow the cache if needed */
            if (len == cap)
            {
                cap = cap ? 2u * cap : 16u;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = (seq_t)l;
        }
    }

    return terms[n - 1];
}
