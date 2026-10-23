/*
 * A048798  Smallest k > 0 such that n*k is a perfect cube.
 * https://oeis.org/A048798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^((3-e%3)%3) (smallest k such that n*k is a
 *            perfect cube)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n=0, but return 0 anyway */

    idx_t temp = n;
    seq_t result = 1;

    /* factor 2 separately for speed */
    idx_t exp = 0;
    while (temp % 2 == 0) {
        temp /= 2;
        exp++;
    }
    if (exp) {
        idx_t f = (3 - (exp % 3)) % 3;
        if (f == 1)          /* multiply by 2 */
            result *= (seq_t)2;
        else if (f == 2) {   /* multiply by 2^2 */
            seq_t pseq = (seq_t)2;
            result *= pseq * pseq;
        }
    }

    /* odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            idx_t f = (3 - (e % 3)) % 3;
            if (f == 1) {
                result *= (seq_t)p;
            } else if (f == 2) {
                seq_t pseq = (seq_t)p;
                result *= pseq * pseq;
            }
        }
    }

    /* leftover prime factor (if any) */
    if (temp > 1) {
        /* exponent is 1 → f = 2 */
        seq_t pseq = (seq_t)temp;
        result *= pseq * pseq;
    }

    return result;
}
