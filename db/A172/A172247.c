/*
 * A172247  Numbers k for which there are at least 2 partitions k = x+y with
 *          x<=y and gcd(x,y,k)=1 having the same value N(x,y,n) defined as
 *          the product of distinct prime divisors of x*y*n.
 * https://oeis.org/A172247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer k for which there exist at
 *            least two coprime partitions k = x + y (x <= y) such that
 *            N(x,y,k) = product of distinct prime divisors of x*y*k is equal
 *            for at least two different partitions. The function searches for
 *            such k incrementally
 * Range:     u8 n <= 9, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* Static table of found terms */
    static seq_t *terms = NULL;
    static size_t terms_used = 0, terms_cap = 0;
    static idx_t last_k = 0;      /* largest k processed so far */

    /* Temporary array to store N values for one k */
    static uint64_t *seen = NULL;
    static size_t seen_used = 0, seen_cap = 0;

    /* Keep searching until we have at least n terms */
    while (terms_used < n)
    {
        ++last_k;              /* next integer to test */

        bool good = false;
        seen_used = 0;         /* clear N list for this k */

        /* Enumerate partitions x + y = last_k with x <= y */
        for (idx_t x = 1; x <= last_k / 2; ++x)
        {
            idx_t y = last_k - x;

            /* Check coprimality gcd(x,y,last_k)=1 */
            seq_t g = gcd((seq_t)x, (seq_t)y);
            g = gcd(g, (seq_t)last_k);
            if (g != (seq_t)1)
                continue;

            /* Compute N = rad(x*y*last_k) */
            uint64_t prod = (uint64_t)x * y;
            prod *= (uint64_t)last_k;   /* prod <= about 8e12 */

            uint64_t N = (uint64_t)rad((idx_t)prod);

            /* Search for a duplicate N in the list seen so far */
            for (size_t i = 0; i < seen_used; ++i)
            {
                if (seen[i] == N)
                {
                    good = true;
                    break;
                }
            }
            if (good)
                break;

            /* Append N to the list, increasing capacity if needed */
            if (seen_used == seen_cap)
            {
                size_t new_cap = seen_cap ? seen_cap * 2 : 16;
                seen = (uint64_t *)realloc(seen, new_cap * sizeof(uint64_t));
                seen_cap = new_cap;
            }
            seen[seen_used++] = N;
        }

        /* If this k satisfies the condition, store it */
        if (good)
        {
            if (terms_used == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
            terms[terms_used++] = (seq_t)last_k;
        }
    }

    /* Return the requested term */
    return terms[n - 1];
}
