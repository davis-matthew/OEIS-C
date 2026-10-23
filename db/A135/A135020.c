/*
 * A135020  Each natural number is followed by its reversal.
 * https://oeis.org/A135020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = (n+1)/2 for odd n, a(n) = reverse_decimal(n/2) for even n
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n & 1) {
        /* n is odd */
        return ((seq_t)n + 1) / 2;
    } else {
        /* n is even */
        return reverse_decimal((seq_t)(n >> 1));
    }
}
