/*
 * A189923  Jordan function J_{-5}(n) multiplied by n^5.
 * https://oeis.org/A189923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Product_{p|n} (1 - p^5), a(1) = 1
 * Range:     i8 n <= 2, i16 n <= 9, i32 n <= 73, i64 n <= 6208,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n by trial division up to √n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 1)
        return (seq_t)1;

    /* Result starts at 1 (multiplicative identity) */
    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Trial division for each prime factor */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* Compute p^5 in seq_t */
            seq_t p5 = (seq_t)p;
            p5 *= (seq_t)p;   /* p^2 */
            p5 *= (seq_t)p;   /* p^3 */
            p5 *= (seq_t)p;   /* p^4 */
            p5 *= (seq_t)p;   /* p^5 */

            /* Multiply the factor (1 - p^5) */
            result *= ((seq_t)1 - p5);

            /* Remove all powers of p from temp */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* If a prime factor remains > 1 it is > sqrt(original n) */
    if (temp > 1)
    {
        /* Compute temp^5 in seq_t */
        seq_t p5 = (seq_t)temp;
        p5 *= (seq_t)temp;   /* temp^2 */
        p5 *= (seq_t)temp;   /* temp^3 */
        p5 *= (seq_t)temp;   /* temp^4 */
        p5 *= (seq_t)temp;   /* temp^5 */

        result *= ((seq_t)1 - p5);
    }

    return result;
}
