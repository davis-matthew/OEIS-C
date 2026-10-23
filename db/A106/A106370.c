/*
 * A106370  Smallest b > 1 such that n contains no zeros in its base b
 *          representation.
 * https://oeis.org/A106370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base b>1 such that the base‑b representation of
 *            n contains no zero digits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    for (idx_t b = 2; ; ++b) {
        idx_t temp = n;
        bool ok = true;
        while (temp > 0) {
            /* if any digit is zero, this base fails */
            if ((temp % b) == 0) {
                ok = false;
                break;
            }
            temp /= b;
        }
        if (ok) {
            return (seq_t)b;
        }
    }
}
