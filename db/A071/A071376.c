/*
 * A071376  Duplicate of A064502.
 * https://oeis.org/A071376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n if n is prime a(n) = product of the two distinct primes
 *            p and q with p+q = n such that p*q is minimal (if there are
 *            none, a(n) = 0) For odd composite n, the only possible pair is
 *            (2, n-2); if n-2 is prime, a(n) = 2*(n-2). For even composite n,
 *            iterate all primes p <= n/2, check that n-p...
 * Range:     i8 n <= 23, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    /* Handle very small n */
    if (n < 2)
        return 0;

    /* Prime case */
    if (isPrime(n))
        return (seq_t)n;

    /* Odd composite numbers */
    if ((n & 1) == 1) {   /* n is odd */
        if (isPrime(n - 2))
            return 2 * (seq_t)(n - 2);    /* 2 and (n-2) give the minimal product */
        return 0;
    }

    /* Even composite numbers */
    seq_t minProd = 0;
    bool found = false;

    for (idx_t p = 2; p <= n / 2; ++p) {
        if (!isPrime(p))
            continue;
        idx_t q = n - p;
        if (p >= q)       /* keep distinct primes and avoid double counting */
            continue;
        if (!isPrime(q))
            continue;
        seq_t prod = (seq_t)p * (seq_t)q;
        if (!found || prod < minProd) {
            minProd = prod;
            found = true;
        }
    }

    return found ? minProd : 0;
}
