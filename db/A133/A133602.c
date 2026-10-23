/*
 * A133602  The matrix-vector product A133080 * A000108.
 * https://oeis.org/A133602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n) if n is even * a(n) = Catalan(n)+Catalan(n-1)
 *            if n is odd * where Catalan(k) is the k‑th Catalan number
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)            /* even n */
        return catalan(n);
    else                         /* odd n  */
        return catalan(n) + catalan(n - 1);
}
