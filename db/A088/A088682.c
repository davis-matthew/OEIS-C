/*
 * A088682  a(n) = prime(3*n+1) - prime(3*n-1).
 * https://oeis.org/A088682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(3*n+1) - prime(3*n-1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* compute using the 1‑indexed prime() function */
    return (seq_t)get_prime(3*n + 1) - (seq_t)get_prime(3*n - 1);
}
