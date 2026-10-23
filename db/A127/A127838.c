/*
 * A127838  a(1) = 1, a(2) = a(3) = a(4) = 0; a(n) = a(n-4) + a(n-3) for n >
 *          4.
 * https://oeis.org/A127838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=a(3)=a(4)=0; for n>4: a(n)=a(n-4)+a(n-3)
 * Range:     u8 n <= 38, u16 n <= 66, u32 n <= 122, u64 n <= 233,
 *            u128 n <= 456, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases directly */
    if (n == (idx_t)1) return (seq_t)1;
    if (n <= (idx_t)4) return (seq_t)0;

    /* Rolling buffer holding the last four terms:
       buf[0] = a(i-4), buf[1] = a(i-3), buf[2] = a(i-2), buf[3] = a(i-1)   */
    seq_t buf[4] = { (seq_t)1, /* a1 */
                     (seq_t)0, /* a2 */
                     (seq_t)0, /* a3 */
                     (seq_t)0  /* a4 */ };

    for (idx_t i = (idx_t)5; i <= n; ++i) {
        /* a(i) = a(i-4) + a(i-3) */
        seq_t nxt = buf[0] + buf[1];

        /* Shift the window forward */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = nxt;
    }

    return buf[3];  /* a(n) is now stored in buf[3] */
}
