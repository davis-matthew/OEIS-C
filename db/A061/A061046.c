/*
 * A061046  Denominator of 1/36 - 1/n^2.
 * https://oeis.org/A061046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 1/36 - 1/n^2 a(n) = (36 * n^2) / gcd(1296,
 *            |n^2 - 36|)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Euclidean algorithm for seq_t */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, return 0 */

    /* work in seq_t to avoid overflow */
    seq_t nseq = (seq_t)n;

    /* n^2 */
    seq_t nsq = nseq * nseq;

    /* |n^2 - 36| as seq_t (unsigned) */
    seq_t tmp;
    if (nsq >= (seq_t)36)
        tmp = nsq - (seq_t)36;
    else
        tmp = (seq_t)36 - nsq;

    /* gcd of abs(n^2-36) and 1296 */
    seq_t g = seq_gcd(tmp, (seq_t)1296);

    /* denominator: (36 * n^2) / g */
    return ((seq_t)36 * nsq) / g;
}
