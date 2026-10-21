/*
 * A074830  Number of base reversals which result in a prime for bases less
 *          than n.
 * https://oeis.org/A074830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of bases b (2 ≤ b < n) for which the base‑b
 *            reverse of n is prime
 * Range:     u8 n <= 32, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrime64.h"          /* provides bool isPrime64(uint64_t) */

seq_t a(idx_t n)
{
    if (n <= 2)          /* for n = 1 or 2 there are no bases < n (≥2) */
        return 0;

    seq_t count = 0;

    for (idx_t b = 2; b < n; ++b)
    {
        seq_t rev = 0;             /* reversed number in base b */
        idx_t tmp = n;

        while (tmp)
        {
            idx_t digit = tmp % b;    /* least‑significant digit in base b */
            rev = rev * (seq_t)b + (seq_t)digit;
            tmp /= b;
        }

        if (isPrime64((uint64_t)rev))
            ++count;
    }

    return count;
}
