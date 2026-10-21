/*
 * A083553  Product of prime(n+1)-1 and prime(n)-1.
 * https://oeis.org/A083553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+1)-1)*(prime(n)-1)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, return 0 for n<1 */

    seq_t p_n   = get_prime(n);       /* prime(n)   */
    seq_t p_np1 = get_prime(n + 1);   /* prime(n+1) */

    return (p_n - 1) * (p_np1 - 1);
}
