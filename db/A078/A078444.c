/*
 * A078444  Floor of geometric mean of two consecutive primes.
 * https://oeis.org/A078444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(prime(n) * prime(n+1)))
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 3394, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt_seq, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    /* get the nth and (n+1)th primes */
    seq_t p1 = get_prime(n);
    seq_t p2 = get_prime(n + 1);

    /* product and its floor square‑root */
    seq_t prod = p1 * p2;
    return floor_sqrt_seq(prod);
}
