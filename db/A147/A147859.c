/*
 * A147859  Chromatic polynomial pi_n(z) of the helm graph H_n evaluated at
 *          z=n.
 * https://oeis.org/A147859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*((1-n)^n*(n-2)+(n-2)^n*(n-1)^n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the chromatic polynomial of the helm graph H_n at z=n */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n=1 and n=2 give 0 directly */
    if (n <= 2)
        return (seq_t)0;

    seq_t n_seq = (seq_t)n;

    /* Compute (n-2)^n and (n-1)^n using big_pow */
    seq_t pow_a = big_pow((seq_t)(n - 2), (seq_t)n);
    seq_t pow_b = big_pow((seq_t)(n - 1), (seq_t)n);

    /* B = (n-2)^n * (n-1)^n */
    seq_t B = pow_a * pow_b;

    /* |A| = (n-1)^n * (n-2)  (absolute value of (1-n)^n * (n-2)) */
    seq_t absA = pow_b * (seq_t)(n - 2);

    /* Sign of A: base (1-n) is negative for n>1; negative^n is negative when n is odd */
    bool negA = (n % 2) == 1;

    seq_t sum;
    if (negA)
    {
        /* B - |A|  (B >= |A| for all n>=3) */
        sum = B - absA;
    }
    else
    {
        /* B + |A| */
        sum = B + absA;
    }

    /* Return n * sum */
    return n_seq * sum;
}
