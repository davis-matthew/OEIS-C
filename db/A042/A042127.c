/*
 * A042127  Denominators of continued fraction convergents to sqrt(588).
 * https://oeis.org/A042127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 194*a(n-2) - a(n-4) for n>3, with a(0)=1, a(1)=4,
 *            a(2)=193, a(3)=776
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four values explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)193;
    if (n == 3) return (seq_t)776;

    /* Use a rolling buffer of the last four terms. */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(0)  */
    buf[1] = (seq_t)4;   /* a(1)  */
    buf[2] = (seq_t)193; /* a(2)  */
    buf[3] = (seq_t)776; /* a(3)  */

    /* Iterate from 4 to n computing a(i) = 194*a(i-2) - a(i-4). */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = ((seq_t)194) * buf[(i - 2) & 3] - buf[(i - 4) & 3];
        buf[i & 3] = next;
    }

    return buf[n & 3];
}
