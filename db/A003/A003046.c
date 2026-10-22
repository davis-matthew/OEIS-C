/*
 * A003046  Product of first n Catalan numbers.
 * https://oeis.org/A003046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C(0)*C(1)*...*C(n), C(k) = A000108(k), C(0) = 1, C(k+1) =
 *            C(k)*(4k+2)/(k+2); product taken with a balanced product tree
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   a003046_prodcat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003046_prodcat.h"

seq_t a(idx_t n)
{
    seq_t c = (seq_t)1;
    return a003046_prodcat(0, n, &c);
}
