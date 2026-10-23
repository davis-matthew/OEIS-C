/*
 * A100196  Number of positive integer cubes <= n^2.
 * https://oeis.org/A100196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n^(2/3)) = floor(cuberoot(n*n))
 * Range:     u8 n <= 4095, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n2 = n * n;  use 128-bit arithmetic to avoid overflow of the
       intermediate product.  In the final return we cast back to seq_t
       because the result is always <= n. */
    unsigned __int128 n2 = (unsigned __int128)n * n;

    idx_t low = 0;
    idx_t high = n;

    while (low < high)
    {
        /* mid = ceil((low + high) / 2) to avoid infinite loop when
           low+1 == high. */
        idx_t mid = (low + high + 1) >> 1;

        /* Compute mid^3 safely:
           First compute mid^2 as 128-bit.  If mid is zero, mid^3 is zero.
           Then check whether mid^3 would overflow and exceed n2.  Finally
           compare with n2. */
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        bool midLarge;

        if (mid == 0)
        {
            midLarge = false;                 /* 0^3 = 0 <= n2 */
        }
        else if (mid2 > n2 / mid)
        {
            /* mid2 * mid would overflow past n2 */
            midLarge = true;
        }
        else
        {
            unsigned __int128 mid3 = mid2 * mid;  /* safe to multiply */
            midLarge = mid3 > n2;
        }

        if (!midLarge)
        {
            low = mid;         /* mid^3 <= n2, answer is at least mid */
        }
        else
        {
            high = mid - 1;    /* mid^3 > n2, answer is below mid */
        }
    }

    return (seq_t)low;
}
