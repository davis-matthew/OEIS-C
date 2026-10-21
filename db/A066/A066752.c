/*
 * A066752  a(n) = gcd(prime(n)+1, n+1).
 * https://oeis.org/A066752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(prime(n)+1, n+1)
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑indexed) */
    seq_t p = (seq_t) get_prime(n);

    /* Compute gcd(prime(n)+1, n+1) */
    seq_t term = (seq_t)(n + 1);
    return gcd(p + 1, term);
}
