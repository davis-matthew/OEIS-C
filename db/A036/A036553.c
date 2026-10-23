/*
 * A036553  a(n) = phi(prime(n)) - prime(phi(n)).
 * https://oeis.org/A036553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = phi(prime(n)) - prime(phi(n))
 * Range:     i8 n <= 41, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   get_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, but to be safe handle n==0 gracefully. */
    if (n == 0)
        return 0;

    /* nth prime, n ≥ 1 */
    idx_t p = get_prime(n);            /* prime(n)   → uint32_t → idx_t */
    seq_t phi_n = phi(n);              /* φ(n) for the index n            */

    /* φ(p) where p is a prime; this equals p-1 */
    seq_t phi_p = phi(p);

    /* q = prime(φ(n)) */
    idx_t q = get_prime((idx_t)phi_n);

    /* return φ(p) – prime(φ(n)) */
    return phi_p - (seq_t)q;
}
