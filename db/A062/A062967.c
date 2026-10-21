/*
 * A062967  a(n) = 2*(sigma(n)-n-1)+1, where sigma = A000203, sum of divisors
 *          of n.
 * https://oeis.org/A062967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*(sigma(n)-n-1)+1 (offset 1)
 * Range:     i8 n <= 47, i16 n <= 6719, i32 n <= 8721, i64 n <= 8721,
 *            i128 n <= 8721, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    idx_t tmp = n;               /* local copy for factorisation */
    seq_t sigma = (seq_t)1;      /* σ(1) = 1 */

    /* Trial division up to √tmp */
    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            /* count exponent of prime p */
            unsigned int exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }

            /* compute the factor (p^(exp+1)-1)/(p-1) as a geometric sum */
            seq_t term = (seq_t)1;          /* the sum for this prime factor */
            seq_t p_seq = (seq_t)p;
            for (unsigned int i = 0; i < exp; ++i)
                term = term * p_seq + (seq_t)1;

            sigma = sigma * term;          /* multiply contributions */
        }
    }

    /* Remaining prime factor, if any */
    if (tmp > 1)
    {
        seq_t p_seq = (seq_t)tmp;
        seq_t term = p_seq + (seq_t)1;    /* (p^2-1)/(p-1) = p+1 */
        sigma = sigma * term;
    }

    /* a(n) = 2*(sigma(n)-n-1)+1 */
    seq_t diff = sigma - (seq_t)n - (seq_t)1;
    return (seq_t)( (diff << 1) + (seq_t)1 );
}
