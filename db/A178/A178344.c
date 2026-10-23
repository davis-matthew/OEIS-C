/*
 * A178344  a(n) = Sum_i prime(i+1)^b(i) where n = Sum_{i>=0} b(i)*2^e(i) is
 *          the binary representation of n.
 * https://oeis.org/A178344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the sum of prime(i+1)^{b(i)} for n represented in binary.
 *            Here b(i) is the i-th bit of n (0 or 1). For a bit equal to 0 we
 *            add 1 (prime(i+1)^0 = 1); for a bit equal to 1 we add the
 *            corresponding prime. The highest considered bit is the position
 *            of the most significant 1 in n. By definition...
 * Range:     u8 n <= 9747, u16 n <= 9747, u32 n <= 9747, u64 n <= 9747,
 *            u128 n <= 9747, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(0) is given as 1 in the OEIS data. */
    if (n == 0) return (seq_t)1;

    idx_t tmp   = n;
    idx_t bit   = 0;
    seq_t sum   = 0;

    while (tmp != 0) {
        if (tmp & 1ULL)                /* bit = 1 → add prime(bit+1) */
            sum += (seq_t)get_prime(bit + 1);
        else                           /* bit = 0 → add 1 */
            sum += (seq_t)1;
        tmp >>= 1ULL;
        ++bit;
    }
    return sum;
}
