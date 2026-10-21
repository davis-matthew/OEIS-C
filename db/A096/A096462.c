/*
 * A096462  Sum of index values of the prime factors (with multiplicity) of n.
 * https://oeis.org/A096462
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{p^e‖n} ( e * Σ_{i≥1} ⌊(n−1)/p^i⌋ + e·(e+1)/2 )
 * Range:     u8 n <= 63, u16 n <= 6143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n by trial division and uses the closed‑form
   formula for the sum of indices of its prime factors. */

#include <stdint.h>
#include <stdbool.h>

/* idx_t and seq_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* offset starts at n = 2 */

    idx_t n_minus_one = n - 1;
    idx_t temp = n;
    seq_t result = 0;

    /* Factor n by trial division. */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* Count the number of occurrences of p in 1..n-1. */
            idx_t t = n_minus_one / p;
            seq_t before = 0;
            while (t > 0)
            {
                before += (seq_t)t;  /* cast to seq_t for the sum */
                t /= p;
            }

            /* Add the contribution of this prime power. */
            result += (seq_t)e * before + (seq_t)(e * (e + 1) / 2);
        }
    }

    /* If a prime > sqrt(n) remains. */
    if (temp > 1)
    {
        idx_t p = temp;               /* prime factor */
        int e = 1;                    /* exponent is 1 */

        idx_t t = n_minus_one / p;
        seq_t before = 0;
        while (t > 0)
        {
            before += (seq_t)t;
            t /= p;
        }

        result += (seq_t)e * before + (seq_t)(e * (e + 1) / 2);
    }

    return result;
}
