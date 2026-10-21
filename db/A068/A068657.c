/*
 * A068657  Successive left concatenation of floor(k/2) beginning with n until
 *          we reach 1.
 * https://oeis.org/A068657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = left‑concatenation of floor(k/2) starting with n until 1
 *            (Base‑10)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 63, u64 n <= 639,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    idx_t cur     = n;

    while (true) {
        /* Convert current integer part to seq_t */
        seq_t part = (seq_t)cur;

        /* Determine 10^d where d is number of decimal digits of 'part' */
        seq_t pow10 = (seq_t)1;
        seq_t temp  = part;
        unsigned int digits = 0;
        do {
            ++digits;
            temp /= (seq_t)10;
        } while (temp != (seq_t)0);
        for (unsigned int i = 0; i < digits; ++i)
            pow10 *= (seq_t)10;

        /* Append the part to the running result */
        result = result * pow10 + part;

        if (cur == (idx_t)1)
            break;
        cur /= (idx_t)2;
    }

    return result;
}
