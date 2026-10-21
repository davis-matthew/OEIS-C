/*
 * A058384  Largest power of 2 which is a divisor of p(n)-1, where p(n) = n-th
 *          prime.
 * https://oeis.org/A058384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest power of 2 that divides p(n)-1, where p(n) is the
 *            n-th prime (offset 1)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* If n < 1 we return 0; behaviour for indices below the offset
       does not matter according to the problem statement. */
    if (n < 1)
        return 0;

    /* Get the n-th prime.  get_prime returns a 32‑bit unsigned prime. */
    uint32_t p = get_prime(n);

    /* Compute p-1 and find its largest power of two divisor
       by shifting out all trailing zeros. */
    seq_t m = (seq_t)p - 1;   /* p-1  */
    seq_t power = 1;          /* power of two factor */

    while ((m & 1) == 0) {    /* while the least significant bit is 0 */
        power <<= 1;          /* multiply power by 2 */
        m >>= 1;              /* shift right to remove that zero bit */
    }
    return power;             /* largest power of 2 dividing p-1 */
}
