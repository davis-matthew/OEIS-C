/*
 * A076480  a(n) = n + mu(rad(n)), where mu is the Moebius function (A008683)
 *          and rad is the squarefree kernel (A007947).
 * https://oeis.org/A076480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + mu(rad(n))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t m = n;                     /* temporary copy for factorisation */
    int parity = 0;                  /* 0 = even number of distinct primes, 1 = odd */

    /* factor 2 */
    if ((m & 1ULL) == 0) {
        parity ^= 1;                 /* one distinct prime (2) */
        while ((m & 1ULL) == 0) {
            m >>= 1;                 /* divide out all 2's */
        }
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            parity ^= 1;             /* found a new distinct prime */
            while (m % p == 0) {
                m /= p;              /* skip repeated factors */
            }
        }
    }

    /* remaining prime greater than sqrt(original n) */
    if (m > 1) {
        parity ^= 1;
    }

    seq_t result = (seq_t)n;
    if (parity == 0) {
        result += 1;                 /* even number of distinct primes */
    } else {
        result -= 1;                 /* odd number of distinct primes */
    }
    return result;
}
