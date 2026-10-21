/*
 * A230629  a(0) = 0; thereafter a(n) = (1 + a(floor(n/2))) mod 3.
 * https://oeis.org/A230629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = (floor(log2(n)) + 1) mod 3. This
 *            follows from the recurrence a(n) = (1 + a(floor(n/2))) mod 3
 * Range:     u8 n <= 7900, u16 n <= 7900, u32 n <= 7900, u64 n <= 7900,
 *            u128 n <= 7900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Count shifts needed to bring n to 1; this equals floor(log2(n)). */
    unsigned int shifts = 0;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        ++shifts;
    }

    unsigned int len = shifts + 1;
    return (seq_t)(len % 3);
}
