/*
 * A109585  a(n) = ( prime(n+1) - prime(n) )^(n+1).
 * https://oeis.org/A109585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+1)-prime(n))^(n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 41,
 *            big any n
 * Helpers:   get_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Get consecutive primes p_n and p_{n+1} */
    seq_t p1 = get_prime(n);
    seq_t p2 = get_prime(n + 1);

    /* Difference of the primes */
    seq_t diff = p2 - p1;

    /* Raise the difference to the (n+1)-th power */
    return pow_seq(diff, n + 1);
}
