/*
 * A061742  a(n) is the square of the product of first n primes.
 * https://oeis.org/A061742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = ( ∏_{k=1..n} p_k )^2 where p_k is the k-th prime * (a(0)
 *            = 1). The implementation uses get_nth_prime(idx_t n) * provided
 *            by the utilities. It runs in O(n) multiplications and * works
 *            for any build (uint8_t … unsigned __int128 …
 *            arbitrary‑precision)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   get_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_nth_prime.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    seq_t primorial = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        primorial *= get_nth_prime(i);
    }
    return primorial * primorial;  /* square of the primorial */
}
