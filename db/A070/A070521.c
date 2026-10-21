/*
 * A070521  Value of prime(n)-th cyclotomic polynomial at n.
 * https://oeis.org/A070521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^p - 1)/(n - 1) where p = prime(n), with a(1)=2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)        /* special case: a(1) = 2 */
        return (seq_t)2;

    seq_t N = (seq_t)n;                       /* the variable x in Φ_p(x) */
    seq_t p = (seq_t)get_prime(n);            /* p = prime(n) */
    seq_t power = big_pow(N, p);              /* compute N^p */
    return (power - (seq_t)1) / (N - (seq_t)1);/* evaluate geometric sum */
}
