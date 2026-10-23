/*
 * A156769  a(n) = denominator(2^(2*n-2)/factorial(2*n-1)).
 * https://oeis.org/A156769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = denominator( 2^(2*n-2) / factorial(2*n-1) )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t den = (seq_t)1;
    for (idx_t k = 1; k < n; ++k) {
        idx_t oddk = k;
        /* strip all factors of 2 from k */
        while ((oddk & 1) == 0)
            oddk >>= 1;
        den *= (seq_t)oddk;          /* multiply by the odd part of k   */
        den *= (seq_t)(2*k + 1);     /* multiply by (2k+1), always odd   */
    }
    return den;
}
