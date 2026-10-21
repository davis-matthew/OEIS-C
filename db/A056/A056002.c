/*
 * A056002  a(n) = (10^2)*11^(n-2); a(0)=1, a(1)=9.
 * https://oeis.org/A056002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 100*11^(n-2); a(0)=1, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow11.h"

seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* For n >= 2: a(n) = 100 * 11^(n-2). */
    seq_t power = pow11(n - 2);
    return power * (seq_t)100;
}
