/*
 * A081432  RevDecimal(RevBinary(n)), where RevDecimal(m) is the decimal
 *          reversal of m (A004086) and RevBinary(m) is the binary reversal of
 *          m (A030101).
 * https://oeis.org/A081432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = revdecimal(revbinary(n))
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   revBinary, revDecimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A081432: a(n) = A004086(A030101(n)) */
/* RevBinary reverses the binary digits of n (without leading zeros).  */
/* RevDecimal reverses the decimal digits of a non‑negative integer.  */

#include <stdint.h>
#include "../utils/revBinary.h"
#include "../utils/revDecimal.h"

seq_t a(idx_t n)
{
    return revDecimal(revBinary(n));
}
