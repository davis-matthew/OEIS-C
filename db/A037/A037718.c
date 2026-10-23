/*
 * A037718  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1,3.
 * https://oeis.org/A037718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the base‑6 number whose first n digits are the periodic
 *            * sequence 2,0,1,3. Hence a(n) = a(n‑1)*6 + digit_n, * where
 *            digit_n = {2,0,1,3}[(n‑1) mod 4], and a(1)=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* periodic base‑6 digits */
    static const seq_t digits[4] = { (seq_t)2, (seq_t)0, (seq_t)1, (seq_t)3 };
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        res = res * (seq_t)6 + digits[(i - 1) % 4];
    }
    return res;
}
