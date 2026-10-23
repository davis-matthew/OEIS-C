/*
 * A188641  Characteristic function of Niven (or Harshad) numbers.
 * https://oeis.org/A188641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n is a Harshad (Niven) number, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t sum  = 0;
    while (temp > 0) {
        sum += temp % 10;
        temp /= 10;
    }
    /* sum is non‑zero for n >= 1 */
    return (seq_t)(n % sum == 0);
}
