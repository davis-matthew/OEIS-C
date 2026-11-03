/*
 * A216119  Number of stretching pairs in all permutations in S_n.
 * https://oeis.org/A216119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*(n-2)*(n-3)/24 (OEIS A216119)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 405 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is 0. */
    if (n < 4)
        return 0;

    /* We compute the product
         n! * (n-2) * (n-3)
    divided by 24,
       cancelling factors of 24 while multiplying.
       This keeps intermediate values within the range of seq_t
       for 64‑bit builds and works for arbitrary‑precision builds
       because seq_mul and gcd are provided.  */
    seq_t result = 1;
    seq_t div = 24;                /* divisor that must be cancelled completely */

    /* Multiply the factorial part.  */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t m = (seq_t)i;

        /* Cancel a common divisor of m and div first.  */
        seq_t g = gcd(m, div);
        if (g > 1)
        {
            m   /= g;
            div /= g;
        }

        /* Multiply into result.  */
        result = seq_mul(result, m);

        /* After the multiplication, try to cancel any remaining part of div from the
           product that's already in result.  This keeps the value small.  */
        if (div > 1)
        {
            seq_t g2 = gcd(result, div);
            if (g2 > 1)
            {
                result /= g2;
                div    /= g2;
            }
        }
    }

    /* Multiply the extra factors (n-2) and (n-3).  */
    idx_t extra[2] = { n - 2, n - 3 };
    for (int k = 0; k < 2; ++k)
    {
        seq_t m = (seq_t)extra[k];
        seq_t g = gcd(m, div);
        if (g > 1)
        {
            m   /= g;
            div /= g;
        }
        result = seq_mul(result, m);

        if (div > 1)
        {
            seq_t g2 = gcd(result, div);
            if (g2 > 1)
            {
                result /= g2;
                div    /= g2;
            }
        }
    }

    /* At the end div should be 1.  If it's larger, something went wrong,
       but for all valid inputs it will be exactly cancelled.  */
    if (div != 1)
        result /= div;   /* final safety – should be a no‑op */

    return result;
}
