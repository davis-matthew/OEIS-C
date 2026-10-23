/*
 * A102487  Numbers in base-12 representation that can be written with decimal
 *          digits.
 * https://oeis.org/A102487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{i>=0} d_i * 12^i where d_i are the decimal digits of
 *            (n-1)
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) = 0, so we use n-1 as the base‑10 number. */
    if (n == 0) return 0;          /* safety, not needed for offset 1 */
    idx_t  num  = n - 1;           /* base‑10 integer to be interpreted in base 12   */
    seq_t result = 0;              /* accumulated value in seq_t                     */
    seq_t power  = 1;              /* 12^i, starting with 12^0 = 1                  */

    while (num > 0)
    {
        uint8_t digit = num % 10;   /* decimal digit 0‑9                           */
        num /= 10;
        result += (seq_t)digit * power;
        power  *= (seq_t)12;        /* advance to next base‑12 place              */
    }
    return result;
}
