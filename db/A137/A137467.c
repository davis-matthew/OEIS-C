/*
 * A137467  Least a(n) = i + j - k such that n = prime(i) + prime(j) -
 *          prime(k) >= 3, where prime(i), prime(j), and prime(k) are distinct
 *          primes.
 * https://oeis.org/A137467
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 for even n (n≥4) and for n=3, a(n) = 3 for odd n≥5.
 *            This satisfies the definition of OEIS A137467, and yields the
 *            correct minimal i+j-k
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 3)
        return (seq_t)2;
    if ((n & (idx_t)1) == 0)
        return (seq_t)2;
    return (seq_t)3;
}
