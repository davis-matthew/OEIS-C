/*
 * A065745  Sum of squares and twice squares dividing n.
 * https://oeis.org/A065745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   multiplicative with * a(2^e) = 2^(e+1)-1 * a(p^e) =
 *            (p^(e+2)-1)/(p^2-1) if e is even * = (p^(e+1)-1)/(p^2-1) if e is
 *            odd, p odd
 * Range:     u8 n <= 24, u16 n <= 960, u32 n <= 9543, u64 n <= 9543,
 *            u128 n <= 9543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* compute p^exp in seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; i++)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = 1;
    idx_t m = n;

    /* factor out powers of 2 */
    idx_t e2 = 0;
    while ((m & 1ULL) == 0) {
        e2++;
        m >>= 1;
    }
    if (e2 > 0) {
        /* a(2^e) = 2^(e+1)-1 */
        seq_t term = pow_seq_t(2, e2 + 1) - 1;
        result *= term;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            idx_t powExp = (e % 2 == 0) ? e + 2 : e + 1;
            seq_t termNum = pow_seq_t((seq_t)p, powExp) - 1;
            seq_t denom = (seq_t)p * (seq_t)p - 1; /* p^2 - 1 */
            result *= termNum / denom;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        idx_t p = m;
        idx_t e = 1;                /* since m was prime */
        idx_t powExp = (e % 2 == 0) ? e + 2 : e + 1; /* e is 1 => powExp = 2 */
        seq_t termNum = pow_seq_t((seq_t)p, powExp) - 1;
        seq_t denom = (seq_t)p * (seq_t)p - 1;
        result *= termNum / denom;
    }

    return result;
}
