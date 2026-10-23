/*
 * A157048  a(n) = the smallest integer > p(n) that is coprime to
 *          (p(n+1)-p(n)), where p(n) is the n-th prime.
 * https://oeis.org/A157048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer > p(n) that is coprime to
 *            (p(n+1)-p(n))
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

/* Return the n-th term of OEIS sequence A157048.  The index n is
   1‑based; values are computed using the first n prime numbers. */
seq_t a(idx_t n)
{
    /* behaviour for n < 1 is irrelevant for the OEIS offset */
    if (n < 1)
        return 0;

    /* get the n‑th and (n+1)‑st prime numbers (1‑based in get_prime) */
    seq_t p_n   = get_prime(n);
    seq_t p_np1 = get_prime(n + 1);

    /* difference between consecutive primes */
    seq_t diff = p_np1 - p_n;

    /* candidate > p_n */
    seq_t k = p_n + 1;

    /* Find the first k > p_n that is coprime to diff.  gcd() is
       provided by "../utils/gcd.h". */
    while (gcd(k, diff) != (seq_t)1)
        ++k;

    return k;
}
