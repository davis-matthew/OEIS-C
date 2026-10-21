/*
 * A086023  a(n) = Sum_{i=1..n} C(i+3,4)^2.
 * https://oeis.org/A086023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(35*n^4 +280*n^3 +685*n^2
 *            +500*n +12) / 181440
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 28, u64 n <= 355, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 1 the OEIS offset is 1, but we return 0 for safety. */
    if (n == 0)
        return 0;

    /* Use 128‑bit arithmetic for all intermediate steps.  The result will
       fit into seq_t for every n that fits into the target type.  For
       arbitrary‑precision builds seq_t can be constructed from an
       unsigned __int128 value. */
    unsigned __int128 nn = n;                     /* n as 128‑bit      */
    unsigned __int128 t1 = nn * (nn + 1) * (nn + 2) * (nn + 3) * (nn + 4);
    unsigned __int128 poly = 35 * nn * nn * nn * nn
                            + 280 * nn * nn * nn
                            + 685 * nn * nn
                            + 500 * nn
                            + 12;
    unsigned __int128 num = t1 * poly;
    const unsigned __int128 denom = 181440u;
    unsigned __int128 result = num / denom;

    return (seq_t)result;
}
