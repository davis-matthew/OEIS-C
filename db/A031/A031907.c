/*
 * A031907  a(n) = prime(7*n - 5).
 * https://oeis.org/A031907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(7*n - 5)
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* By definition n >= 1 */
    idx_t idx = 7*n - 5;          /* index of the desired prime  */
    uint32_t p = get_prime(idx);  /* get_prime returns the kth prime (k >= 1) */
    return (seq_t)p;              /* cast to the required return type */
}
