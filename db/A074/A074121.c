/*
 * A074121  Group successively larger composite numbers so that the sum of the
 *          n-th group is a multiple of n. Sequence gives the first term in
 *          each group.
 * https://oeis.org/A074121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Group successively larger composite numbers so that the sum of
 *            the n-th group is a multiple of n. Sequence gives the first term
 *            in each group
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* memoisation storage: a[1] is the first value, a[2] the second, etc. */
    static seq_t *values = NULL;          /* array of computed a(i) values               */
    static size_t len = 0;                /* number of values stored (excluding index 0) */
    static size_t cap = 0;                /* current allocated capacity                  */
    static idx_t next_comp = 4;           /* next candidate composite number             */

    if (n < 1)
        return 0;                         /* offset is 1; behaviour below is irrelevant */

    /* compute values up to n if necessary */
    while (len < (size_t)n)
    {
        /* Determine the group index */
        idx_t group = (idx_t)(len + 1);

        /* Find the first unused composite number */
        idx_t start = next_comp;
        while (start < 4 || is_prime(start))
            ++start;                      /* skip primes and numbers < 4 */

        /* Group construction */
        idx_t m = start;
        seq_t sum = 0;
        do
        {
            /* current number is composite by construction */
            sum += (seq_t)m;
            ++m;

            /* Advance m to the next composite */
            while (m < 4 || is_prime(m))
                ++m;
        } while ((sum % (seq_t)group) != 0);

        /* Group successfully finished.  The first value of this group is 'start'. */
        next_comp = m;                    /* the next starting point for the following group */

        /* Store the computed value */
        if (len == cap)
        {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* out of memory */
            values = tmp;
            cap = new_cap;
        }
        values[len++] = start;
    }

    return values[n - 1];
}
