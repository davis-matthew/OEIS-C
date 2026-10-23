/*
 * A103543  Consider those values of k for which A102370(k) = k: 0, 4, 8, 16,
 *          20, 24, 32, 36, 40, 48, 52, 56, 64, ... and divide by 4: 0, 1, 2,
 *          4, 5, 6, 8, 9, 10, 12, 13, 14, 16, 17, 18, 20, 21, 22, 24, ...;
 *          sequence gives missing numbers.
 * https://oeis.org/A103543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n-th positive integer that is either (i) congruent
 *            to 3 (mod 4) or (ii) of the form j·2^(4k−2) − k with k ≥ 2, j ≥
 *            1 (the A103584 terms). The function enumerates integers in
 *            increasing order, stopping when n such integers have been found.
 *            This satisfies the required performance for n...
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; this case should not occur */
        return (seq_t)0;

    idx_t count = 0;          /* how many terms have been found so far */
    for (idx_t x = 3; ; ++x) /* start at the smallest possible term */
    {
        int keep = (x & 3) == 3;   /* (i) n ≡ 3 (mod 4) */

        if (!keep)                      /* (ii) possible special term */
        {
            unsigned __int128 pow2 = ((unsigned __int128)1) << 6; /* 2^(4·2−2) */
            for (idx_t k = 2; ; ++k)
            {
                unsigned __int128 val = (unsigned __int128)(x + k);

                /* When the power exceeds val, no larger k will work */
                if (pow2 > val) break;

                if (val % pow2 == 0)   /* x == j·2^(4k−2) − k  */
                {
                    keep = 1;
                    break;
                }

                /* advance to the next k: exponent increases by 4 */
                pow2 <<= 4;
                /* pow2 stays within 128 bits because val ≤ 2^64,
                   so the loop will terminate before overflow occurs. */
            }
        }

        if (keep)                     /* the current x belongs to the sequence */
        {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
