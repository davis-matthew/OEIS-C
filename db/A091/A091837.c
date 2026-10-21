/*
 * A091837  Smallest b>1 such that 11*n is a repdigit in base b.
 * https://oeis.org/A091837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest base b>1 such that 11*n appears as a repdigit in base b. */

#include <stdbool.h>              /* for bool, true, false */

seq_t a(idx_t n)
{
    /* 11*n in the generic integer type */
    seq_t M = (seq_t)n * 11;

    /* Search bound:
       – For n < 13 we use a generous bound of 30 (captures all small cases),
       – For n ≥ 13 the known property a(n) ≤ n-1 holds.                 */
    idx_t limit = (n < 13) ? 30ULL : (n - 1UL);

    for (idx_t bi = 2; bi <= limit; ++bi)
    {
        seq_t b   = (seq_t)bi;           /* current base */
        seq_t rem = M % b;               /* digit d = least‑significant digit */

        /* rem == 0 would make the leading digit 0 – impossible for a
           positive integer, so skip such bases.                 */
        if (rem == 0)
            continue;

        seq_t t   = M;
        bool repdigit = true;

        /* Check that every digit equals rem. */
        while (t > 0)
        {
            if (t % b != rem)
            {
                repdigit = false;
                break;
            }
            t /= b;
        }

        if (repdigit)
            return b;                    /* smallest base found */
    }

    /* Fallback – the theorem guarantees a solution before this point. */
    return (seq_t)(limit + 1);
}
