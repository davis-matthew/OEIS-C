/*
 * A079627  a(n) = Sum_{prime(n) < m < prime(n+1)} mu(m), where mu is the
 *          Moebius function (A008683).
 * https://oeis.org/A079627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{prime(n) < m < prime(n+1)} μ(m)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1).  Behaviour for n==0 is irrelevant. */
    if (n == 0) return 0;

    /* Fetch the n-th and (n+1)-th primes. */
    uint32_t pn = get_prime(n);      /* prime(n)   */
    uint32_t pn1 = get_prime(n + 1); /* prime(n+1) */

    idx_t p = (idx_t)pn;
    idx_t q = (idx_t)pn1;

    /* Sum the Möbius function over the open interval (p, q). */
    seq_t sum = 0;
    for (idx_t m = p + 1; m < q; ++m) {
        sum += mobius(m);
    }
    return sum;
}
