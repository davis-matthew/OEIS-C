/*
 * A033264  Number of blocks of {1,0} in the binary expansion of n.
 * https://oeis.org/A033264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of “10” transitions in the binary expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(4n) = a(4n+1) = a(2n),
                a(4n+2) = a(n)+1,
                a(4n+3) = a(n).                               */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t cnt = 0u;

    /* Find the index of the most significant 1 bit */
    idx_t temp = n;
    idx_t msb = 0;
    while (temp >> 1) {
        temp >>= 1;
        msb++;
    }

    /* Scan bits from MSB-1 down to 0 */
    int prev = (int)((n >> msb) & 1u);      /* MSB is 1 for n > 0 */

    while (msb > 0) {
        msb--;
        int cur = (int)((n >> msb) & 1u);
        if (prev == 1 && cur == 0)
            cnt++;
        prev = cur;
    }

    return cnt;
}
