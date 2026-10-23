/*
 * A129634  Least nonnegative m such that T(n) + T(m) is prime, where T(n) =
 *          n*(n+1)/2.
 * https://oeis.org/A129634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the least nonnegative m such that T(n) + T(m) is prime,
 *            where T(k) = k*(k+1)/2
 * Range:     u8 n <= 21, u16 n <= 1726, u32 n <= 163, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t fn = (seq_t)n * ((seq_t)n + 1) / 2;   /* triangular number T(n) */
    for (idx_t m = 0;; ++m)                       /* scan m = 0,1,2,... */
    {
        seq_t fm = (seq_t)m * ((seq_t)m + 1) / 2; /* triangular number T(m) */
        seq_t s  = fn + fm;                      /* sum T(n)+T(m)   */
        if (is_prime_big(s))                     /* test primality   */
            return (seq_t)m;                     /* found the minimal m */
    }
}
