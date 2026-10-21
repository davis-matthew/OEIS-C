/*
 * A059630  Carryless multiplication 7 X n base 10.
 * https://oeis.org/A059630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = carryless product of 7 and n in base 10 (each decimal
 *            digit d of n is replaced by (d*7) mod 10, without any carries).
 *            a(n) = Σ_{i≥0} ((digit_i(n) * 7 % 10) * 10^i)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;     /* result of the carryless multiplication */
    seq_t power  = 1;     /* 10^i for the current digit position  */
    idx_t  temp   = n;    /* copy of n for digit extraction         */

    /* Process each decimal digit of n from least significant to most */
    while (temp != 0) {
        unsigned digit = temp % 10;          /* current decimal digit      */
        unsigned prod  = digit * 7;          /* product, up to 63          */
        unsigned digit_res = prod % 10;      /* carryless digit result     */
        result += (seq_t)digit_res * power;  /* add digit to result        */
        power *= 10;                         /* move to next decimal place */
        temp   /= 10;                        /* next digit                 */
    }
    return result;
}
