/*
 * A092929  n-th partial arithmetic mean is equal to the n-th noncomposite
 *          number or to prime(n-1) for n>1.
 * https://oeis.org/A092929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=3; for n>=3: a(n) = n*prime(n-1) - (n-1)*prime(n-2)
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1, but safe */
    if (n == 1) return 1;
    if (n == 2) return 3;
    seq_t p1 = get_prime(n - 1);     /* prime(n-1) */
    seq_t p2 = get_prime(n - 2);     /* prime(n-2) */
    return (seq_t)n * p1 - (seq_t)(n - 1) * p2;
}
