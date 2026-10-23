/*
 * A031100  Write 2n-1 in base 9 and juxtapose.
 * https://oeis.org/A031100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031100: Concatenate the base‑9 representations of odd numbers
   (1, 3, 5, 7, 9, …) and output the n‑th decimal digit (a(n) = 0…8). */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t pos = 1;           /* current index in the infinite digit string   */
    for (idx_t m = 1; ; m += 2)  /* iterate over odd integers                  */
    {
        /* convert m to base‑9 digits (least significant first)   */
        unsigned char digits[32];       /* 32 digits is plenty for any k ≤ 2^64  */
        idx_t i = 0;
        idx_t tmp = m;
        do {
            digits[i++] = tmp % 9;
            tmp /= 9;
        } while (tmp != 0);

        /* output digits in most‑significant first order          */
        for (idx_t k = i; k > 0; --k) {
            if (pos == n)            /* found the requested digit            */
                return (seq_t)digits[k-1];

            ++pos;                    /* advance to next digit                 */
        }
        /* loop continues to next odd m                           */
    }

    /* unreachable but keeps compiler happy                           */
    return (seq_t)0;
}
