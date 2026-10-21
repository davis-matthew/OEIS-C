/*
 * A078644  a(n) = tau(2*n^2)/2.
 * https://oeis.org/A078644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = tau(2*n^2)/2 (tau is the divisor-count function)
 * Range:     u8 n <= 6929, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* the function is pure, no state, O(sqrt(n)) factorisation */
seq_t a(idx_t n)
{
    idx_t t = n;
    seq_t result = 1;
    idx_t c;

    /* handle factor 2 */
    c = 0;
    while ((t & 1ULL) == 0) {      /* t % 2 == 0, use bit test for speed */
        ++c;
        t >>= 1;                   /* t /= 2 */
    }
    if (c)                  /* 2's contribution: (c+1) */
        result *= (seq_t)(c + 1);

    /* odd factors */
    for (idx_t p = 3; p <= t / p; p += 2) {
        c = 0;
        while (t % p == 0) {
            ++c;
            t /= p;
        }
        if (c)                  /* odd prime contribution: (2*c+1) */
            result *= (seq_t)(2 * c + 1);
    }

    /* remaining prime > sqrt(original t) */
    if (t > 1)   /* exponent is 1, contribution (2*1+1) = 3 */
        result *= (seq_t)3;

    return result;
}
