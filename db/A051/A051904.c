/*
 * A051904  Minimal exponent in prime factorization of n.
 * https://oeis.org/A051904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal exponent in the prime factorisation of n * (a(1)
 *            = 0)
 * Range:     u8 n <= 8732, u16 n <= 8732, u32 n <= 8732, u64 n <= 8732,
 *            u128 n <= 8732, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External linkage function for OEIS A051904 */
seq_t a(idx_t n)
{
    if (n == 1)
        return 0;                 /* a(1) = 0 */

    seq_t min_exp = 0;            /* will hold the minimal exponent */
    idx_t temp = n;               /* remaining part of n to factor */

    /* Handle factor 2 separately for speed */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        if (min_exp == 0 || e < min_exp)
            min_exp = e;
    }

    /* Trial division by odd numbers up to sqrt(temp) */
    for (idx_t p = 3; (unsigned __int128)p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (min_exp == 0 || e < min_exp)
                min_exp = e;
        }
    }

    /* If a prime factor > sqrt(original n) remains */
    if (temp > 1) {
        /* Exponent of this prime factor is 1 */
        if (min_exp == 0 || 1 < min_exp)
            min_exp = 1;
    }

    return min_exp;
}
