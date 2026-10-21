/*
 * A079103  a(n) = Catalan(n)^(2*n-2).
 * https://oeis.org/A079103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n)^(2*n-2) (offset 0)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   catalan, mpow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/mpow.h"

/* Computes a(n) for OEIS sequence A079103.
 * For n = 0 or 1 the result is 1.  For n >= 2 it is Catalan(n)
 * raised to the power (2*n-2). */
seq_t a(idx_t n)
{
    if (n <= 1U)          /* a(0) = 1, a(1) = 1 */
        return (seq_t)1;

    seq_t cat = catalan(n);          /* Catalan(n) */
    idx_t exp = n * 2U - 2U;         /* exponent 2*n-2, fits in idx_t */

    return mpow(cat, exp);           /* cat^(2*n-2) */
}
