/*
 * A144841  a(n) = 280*C(n,9)+10*C(n,6)+6*C(n,4)+C(n,3)+1 where C = binomial.
 * https://oeis.org/A144841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 280*C(n,9)+10*C(n,6)+6*C(n,4)+C(n,3)+1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 30, u64 n <= 310,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) for small k.
   Handles k>n by returning 0.  */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k == 0 || n == k)
        return (seq_t)1;
    if (k > n)
        return (seq_t)0;
    if (k > n - k)          /* use symmetry to minimise loops */
        k = n - k;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply first, then divide, relying on exact division
           because binomial coefficients are integers.  */
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute the needed binomial terms */
    seq_t c3 = _binom(n, 3);
    seq_t c4 = _binom(n, 4);
    seq_t c6 = _binom(n, 6);
    seq_t c9 = _binom(n, 9);

    /* Combine with the given coefficients */
    seq_t result = c9 * (seq_t)280 + c6 * (seq_t)10
                 + c4 * (seq_t)6 + c3 + (seq_t)1;
    return result;
}
