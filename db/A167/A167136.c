/*
 * A167136  a(n) = b(n)-th highest positive integer not equal to any a(k), 1
 *          <= k <= n-1, where b(n) = noncomposite numbers = A008578(n).
 * https://oeis.org/A167136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 + prime(n-1) with a(1) = 1 (prime(1)=2, prime(k)
 *            returns the k-th prime)
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for offset 1, but guard anyway */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    /* n >= 2: compute a(n) = (n-1) + prime(n-1) */
    seq_t part = (seq_t)(n - 1);
    uint32_t p = get_prime(n - 1);  /* k-th prime, k >= 1 */
    return part + (seq_t)p;
}
