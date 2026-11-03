/*
 * A126324  a(2n) = Cat(n), a(2n+1) = 3*Cat(n), where Cat(n) =
 *          binomial(2n,n)/(n+1) are the Catalan numbers (A000108).
 * https://oeis.org/A126324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = Catalan(n), a(2n+1) = 3*Catalan(n)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 38, u64 n <= 72, u128 n <= 138,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 3330 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;               /* floor(n/2)   */
    seq_t val = catalan(k);
    if (n & 1U)                     /* odd index    */
        val = val * (seq_t)3;
    return val;
}
