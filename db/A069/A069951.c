/*
 * A069951  Continued fraction expansion of the number e^(2/3).
 * https://oeis.org/A069951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   A069951 (continued fraction of e^(2/3)) a(5k-1) = a(5k) = 1
 *            a(5k+1) = 9k+1 a(5k+2) = 36k+18 a(5k+3) = 9k+7 (with a(0)=1)
 * Range:     u8 n <= 36, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t mod = n % 5;
    seq_t k   = (seq_t)(n / 5);   /* floor(n/5) */

    if (mod == 0 || mod == 4)        /* 5k     or 5k-1 */
        return 1;
    else if (mod == 1)                /* 5k+1   */
        return 9 * k + 1;
    else if (mod == 2)                /* 5k+2   */
        return 36 * k + 18;
    else                               /* mod == 3, 5k+3 */
        return 9 * k + 7;
}
