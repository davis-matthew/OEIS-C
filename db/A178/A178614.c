/*
 * A178614  a(n) = prime(n)!/(n+1)!.
 * https://oeis.org/A178614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)! / (n+1)!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* result = product_{k=n+2}^{prime(n)} k  */
    seq_t result = (seq_t)1;
    idx_t prime_n = get_prime(n);          /* nth prime (uint32_t) */

    for (idx_t k = n + 2; k <= prime_n; ++k)
    {
        result = seq_mul(result, (seq_t)k);
    }

    return result;
}
