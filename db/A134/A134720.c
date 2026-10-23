/*
 * A134720  Even Padovan numbers.
 * https://oeis.org/A134720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-3) + a(n-6) + a(n-9)  for n >= 10
   Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=2, a(6)=4,
                   a(7)=12, a(8)=16, a(9)=28 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first nine terms directly */
    if (n <= 9) {
        static const seq_t base[9] = {0, 0, 0, 2, 2, 4, 12, 16, 28};
        return base[n - 1];
    }

    /* Circular buffer for the last nine terms */
    seq_t buf[9] = {0, 0, 0, 2, 2, 4, 12, 16, 28};

    for (idx_t i = 10; i <= n; ++i) {
        /* Indices into buf for a(i-3), a(i-6), a(i-9) */
        size_t p3 = (size_t)((i - 4) % 9); /* (i-3)-1 */
        size_t p6 = (size_t)((i - 7) % 9); /* (i-6)-1 */
        size_t p9 = (size_t)((i -10) % 9); /* (i-9)-1 */

        seq_t next = 7 * buf[p3] + buf[p6] + buf[p9];
        buf[(size_t)((i - 1) % 9)] = next; /* Store a(i) */
    }

    return buf[(size_t)((n - 1) % 9)];
}
