/*
 * A075888  Difference of successive primes squared divided by 24,
 *          (prime(n+1)^2 - prime(n)^2)/24.
 * https://oeis.org/A075888
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n+1)^2 - prime(n)^2) / 24
 * Range:     u8 n <= 8, u16 n <= 565, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is counted from OEIS offset (a(3) is the first term) */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined – return 0. */
    if (n < 3)
        return 0;

    /* fetch the n-th and (n+1)-th primes */
    seq_t p_n   = get_prime(n);
    seq_t p_np1 = get_prime(n + 1);

    /* compute the difference of squares and divide by 24 */
    seq_t diff = p_np1 * p_np1 - p_n * p_n;
    return diff / 24;
}
