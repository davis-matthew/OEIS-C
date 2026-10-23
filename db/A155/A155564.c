/*
 * A155564  Intersection of A002479 and A003136: N = a^2 + 2b^2 = c^2 + 3d^2
 *          for some integers a,b,c,d.
 * https://oeis.org/A155564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Intersection of A002479 and A003136:
   N = a^2 + 2 b^2 = c^2 + 3 d^2 for some integers a,b,c,d.
   This implementation generates the sets of values up to a
   dynamically increased bound and returns the nth element in
   ascending order (1‑based index). */

#include <stdlib.h>
#include <stdint.h>

/* Note: the build supplies the types seq_t and idx_t. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for unexpected zero index */

    /* Start with a reasonable bound; grow it until we have enough terms. */
    idx_t limit = n * 10 + 100;   /* initial guess */
    while (1)
    {
        /* Allocate bitsets for the two quadratic forms. */
        uint8_t *isA = (uint8_t *)calloc((size_t)(limit + 1), 1);
        uint8_t *isC = (uint8_t *)calloc((size_t)(limit + 1), 1);
        if (!isA || !isC)
        {
            /* Allocation failed – return 0 as a safe fallback. */
            free(isA); free(isC);
            return (seq_t)0;
        }

        /* Mark numbers representable as a^2 + 2 b^2. */
        for (idx_t a0 = 0; a0 * a0 <= limit; ++a0)
        {
            idx_t aa = a0 * a0;
            for (idx_t b0 = 0;; ++b0)
            {
                idx_t val = aa + (idx_t)2 * b0 * b0;
                if (val > limit) break;
                isA[val] = 1;
            }
        }

        /* Mark numbers representable as c^2 + 3 d^2. */
        for (idx_t c0 = 0; c0 * c0 <= limit; ++c0)
        {
            idx_t cc = c0 * c0;
            for (idx_t d0 = 0;; ++d0)
            {
                idx_t val = cc + (idx_t)3 * d0 * d0;
                if (val > limit) break;
                isC[val] = 1;
            }
        }

        /* Scan for the nth intersection element. */
        idx_t count = 0;
        seq_t result = (seq_t)0;
        for (idx_t val = 0; val <= limit; ++val)
        {
            if (isA[val] && isC[val])
            {
                ++count;
                if (count == n)
                {
                    result = (seq_t)val;
                    break;
                }
            }
        }

        free(isA);
        free(isC);

        if (count >= n)
            return result;

        /* Not enough terms – enlarge the bound and retry. */
        limit <<= 1;  /* multiply by 2 */
    }
}
