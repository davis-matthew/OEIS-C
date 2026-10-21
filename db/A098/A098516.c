/*
 * A098516  Minimum wind speed in mph for a category n hurricane in the
 *          Saffir-Simpson hurricane scale.
 * https://oeis.org/A098516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(1) = 74; a(2) = 96; for n > 2, a(n) = a(n-1) + 5*n
 *            (equivalently a(n) = 81 + 5*n*(n+1)/2 for n >= 2)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset is 1, so n == 0 is not used; return 0 for safety */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)74;
    if (n == 2) return (seq_t)96;

    seq_t res = (seq_t)96;               /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        res += (seq_t)5 * i;             /* a(i) = a(i-1) + 5*i */
    }
    return res;
}
