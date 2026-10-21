/*
 * A094896  If 4*n+1 is prime and 4*n+3 is not prime then a(n)=4*n+1, else
 *          a(n)=0.
 * https://oeis.org/A094896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n+1 if 4*n+1 is prime and 4*n+3 is not prime, otherwise
 *            a(n) = 0
 * Range:     u8 n <= 63, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    idx_t v1 = 4 * n + 1;
    idx_t v2 = 4 * n + 3;
    if (is_prime(v1) && !is_prime(v2))
        return (seq_t)v1;
    return 0;
}
