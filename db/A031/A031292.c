/*
 * A031292  Position of n-th 5 in A007376.
 * https://oeis.org/A031292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, base
 * Formula:   position of the n‑th digit 5 in the Champernowne sequence (the
 *            concatenation of the natural numbers 1,2,3,4,…)
 * Range:     u8 n <= 22, u16 n <= 5892, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* target is the remaining number of 5's we need to find */
    idx_t target = n;
    /* current position in the concatenated digit string (1‑based) */
    seq_t pos = 0;

    /* iterate over natural numbers sequentially */
    for (idx_t num = 1; ; ++num)
    {
        /* extract the digits of num into a buffer (reversed order) */
        unsigned char buf[64];
        unsigned char len = 0;
        idx_t tmp = num;
        do
        {
            buf[len++] = (unsigned char)(tmp % (idx_t)10);
            tmp /= (idx_t)10;
        } while (tmp);

        /* process digits from most significant to least significant */
        for (int i = len - 1; i >= 0; --i)
        {
            ++pos;                       /* advance to next digit position */
            if (buf[i] == 5)             /* if this digit is 5 */
            {
                --target;
                if (target == 0)          /* we've found the nth 5 */
                    return pos;
            }
        }
    }
}
