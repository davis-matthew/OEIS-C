/*
 * A084891  Multiples of 2, 3, 5, or 7, but not 7-smooth.
 * https://oeis.org/A084891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer that is a multiple of 2, 3, 5,
 *            or 7 * and possesses at least one prime factor greater than 7
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool (not strictly needed for this implementation) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n<1, but return 0 safely */

    idx_t count = 0;
    seq_t num   = 2;   /* start testing from the first positive integer */

    while (true)
    {
        /* check divisibility by 2,3,5,7 */
        if ((num % 2 == 0) || (num % 3 == 0) || (num % 5 == 0) || (num % 7 == 0))
        {
            /* remove all factors 2,3,5,7 */
            seq_t r = num;
            while (r % 2 == 0) r /= 2;
            while (r % 3 == 0) r /= 3;
            while (r % 5 == 0) r /= 5;
            while (r % 7 == 0) r /= 7;

            if (r > 1)   /* has a prime factor > 7 */
            {
                ++count;
                if (count == n)
                    return num;
            }
        }
        ++num;
    }
}
