/*
 * A144858  Numbers of the form 2^(n-1)*(2^n - 1) which aren't perfect
 *          numbers.
 * https://oeis.org/A144858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n≥2, a(n) = 2^(k-1)*(2^k-1) where k is the (n‑1)-th
 *            positive integer such that 2^k‑1 is NOT prime (i.e., k is not
 *            the exponent of a Mersenne prime)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   isPrimeGeneric, is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;               /* a(1) = 0 by definition */

    idx_t target = n - 1;        /* we look for the (n‑1)-th valid k */
    idx_t count  = 0;

    for (idx_t k = 1; ; ++k)
    {
        bool skip = false;

        /* Only need to test if k is prime; otherwise 2^k‑1 cannot be prime. */
        if (k > 1 && is_prime(k))
        {
            seq_t m = pow2(k);
            m -= 1;            /* m = 2^k - 1 */

            /* Mersenne prime? If yes, the corresponding number is perfect. */
            if (isPrimeGeneric(m))
                skip = true;
        }

        if (!skip)                 /* k gives a non‑perfect number */
        {
            ++count;
            if (count == target)
            {
                seq_t two_k_minus1 = pow2(k - 1);
                seq_t two_k       = pow2(k);
                return two_k_minus1 * (two_k - 1);
            }
        }
    }
}
