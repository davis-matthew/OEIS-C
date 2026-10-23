/*
 * A040250  Continued fraction for sqrt(267).
 * https://oeis.org/A040250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=16; for n≥1, a(n)=period[(n-1)%6] with
 *            period={2,1,15,1,2,32}
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[6] = {
        (seq_t)2, (seq_t)1, (seq_t)15,
        (seq_t)1, (seq_t)2, (seq_t)32
    };

    if (n == (idx_t)0)
        return (seq_t)16;

    idx_t m = n - (idx_t)1;               /* n ≥ 1 here */
    size_t pos = (size_t)(m % (idx_t)6);  /* 0 ≤ pos < 6 */
    return period[pos];
}
