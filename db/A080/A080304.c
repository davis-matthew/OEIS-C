/*
 * A080304  Numerator of n^mu(n), where mu is the Moebius function (A008683).
 * https://oeis.org/A080304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n if μ(n)>0 (n is square‑free with an even number of
 *            prime factors), otherwise 1
 * Range:     u8 n <= 258, u16 n <= 8819, u32 n <= 8819, u64 n <= 8819,
 *            u128 n <= 8819, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t temp = n;
    int mu = 1;                     /* Möbius value, initial sign */

    /* Handle factor 2 */
    if ((temp & 1) == 0) {          /* temp%2==0 */
        temp >>= 1;                  /* divide by 2 */
        if ((temp & 1) == 0) {       /* another 2 found → square factor */
            return (seq_t)1;
        }
        mu = -mu;                   /* one distinct prime factor, flip sign */
    }

    /* Check odd factors up to sqrt(temp) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            temp /= p;               /* divide by p once */
            if (temp % p == 0) {     /* second power → square factor */
                return (seq_t)1;
            }
            mu = -mu;                /* distinct prime factor found */
        }
    }

    /* If anything remains >1, it is a prime factor > sqrt(n) */
    if (temp > 1) mu = -mu;

    return (mu > 0) ? (seq_t)n : (seq_t)1;
}
