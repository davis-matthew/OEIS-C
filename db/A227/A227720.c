/*
 * A227720  Round(1/s(n)), where s(n) = n*log(1+1/n) - (2n-1)/(2n).
 * https://oeis.org/A227720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(1/s(n)) where s(n) = n*log(1+1/n) - (2n-1)/(2n). In
 *            fact a(n) can be expressed exactly in integers as a(n) =
 *            ((12*n*n + 9*n) + c) / 4 where the adjustment c depends on n mod
 *            4: n % 4 == 0 -> c = 0 n % 4 == 1 -> c = -1 n % 4 == 2 -> c = -2
 *            n % 4 == 3 -> c = +1 This formula reproduces...
 * Range:     u8 n <= 4, u16 n <= 146, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 12*n*n + 9*n */
    seq_t base = (seq_t)12 * n * n + (seq_t)9 * n;
    unsigned r = (unsigned)(n % 4);

    /* adjust by c according to n mod 4 */
    if (r == 1)
        base -= (seq_t)1;
    else if (r == 2)
        base -= (seq_t)2;
    else if (r == 3)
        base += (seq_t)1;
    /* r == 0: no adjustment */

    return base / (seq_t)4;
}
