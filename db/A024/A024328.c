/*
 * A024328  a(n) = Sum_{j=1..floor((n+1)/2)} A023531(j)*prime(n-j+1).
 * https://oeis.org/A024328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..floor((n+1)/2)} A023531(j)*prime(n-j+1) where
 *            A023531(j) = 1 if j = k(k+3)/2 for some integer k≥1, else 0
 * Range:     u8 n <= 26, u16 n <= 538, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = (n + 1) / 2;   /* floor((n+1)/2) */
    for (idx_t k = 1; ; ++k)
    {
        idx_t j = (k * (k + 3)) / 2;   /* j = k(k+3)/2  */
        if (j > m) break;
        idx_t prime_idx = n - j + 1;   /* index of prime to add */
        sum += (seq_t)get_prime(prime_idx);
    }
    return sum;
}
