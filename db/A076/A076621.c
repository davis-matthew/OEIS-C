/*
 * A076621  Least square greater than the product of two successive primes.
 * https://oeis.org/A076621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((prime(n)+prime(n+1))/2)^2, with a(1)=9. prime(n) is the
 *            nth prime (1‑based)
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the special first term */
    if (n == 1) return (seq_t)9;

    /* Retrieve the two successive primes */
    seq_t p = get_prime(n);      /* prime(n)  */
    seq_t q = get_prime(n + 1);  /* prime(n+1) */

    /* Compute the average (p+q is even for n>1, so integer division is exact) */
    seq_t avg = (p + q) / 2;

    /* Return the square of the average */
    return avg * avg;
}
