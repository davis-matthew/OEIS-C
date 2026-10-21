/*
 * A227471  Position of first 0 in the binary representation of prime(n),
 *          starting the count of positions at 1 for the least significant
 *          bit.
 * https://oeis.org/A227471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 1 + A023512(n)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* nth prime, n >= 1 */
    unsigned count = 0;
    unsigned long long m = p;           /* use 64‑bit to shift safely */
    while ((m & 1ULL) == 1ULL) {        /* count trailing ones in p */
        ++count;
        m >>= 1;
    }
    return (seq_t)(count + 1);          /* first zero position */
}
