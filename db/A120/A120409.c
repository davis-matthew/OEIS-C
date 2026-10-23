/*
 * A120409  a(n) =
 *          n^1*(n+1)^2*(n+2)^3*(n+3)^4*(n+4)^5*(n+5)^6/(1!*2!*3!*4!*5!*6!).
 * https://oeis.org/A120409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            n^1*(n+1)^2*(n+2)^3*(n+3)^4*(n+4)^5*(n+5)^6/(1!*2!*3!*4!*5!*6!)
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* Denominator prime exponents: 2^12 * 3^5 * 5^2 */
    static const unsigned int denom_primes[3] = {2, 3, 5};
    static const int denom_exps[3] = {12, 5, 2};

    /* Remaining denominator exponents during computation */
    int rem[3];
    for (int i = 0; i < 3; ++i)
        rem[i] = denom_exps[i];

    seq_t result = seq_one();

    /* Multiply (n+k)^(k+1) for k = 0..5, cancelling denominator factors as they appear */
    for (unsigned int k = 0; k < 6; ++k)
    {
        seq_t base = (seq_t)(n + k);
        int exp  = (int)(k + 1);

        for (int e = 0; e < exp; ++e)
        {
            seq_t val = base;

            /* Reduce val by available denominator prime powers */
            for (int pidx = 0; pidx < 3; ++pidx)
            {
                unsigned int p = denom_primes[pidx];
                while (rem[pidx] > 0 && (val % p) == 0)
                {
                    val /= p;
                    rem[pidx]--;
                }
            }

            /* Multiply into the result */
            result *= val;
        }
    }

    /* All denominator factors should have been cancelled; if not, the
       denominator would have been absorbed during the loop. */

    return result;
}
