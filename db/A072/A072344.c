/*
 * A072344  a(n) = the least natural number k such that k*phi(n) + 1 is prime.
 * https://oeis.org/A072344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least natural number k such that k*phi(n) + 1 is
 *            prime
 * Range:     u8 n <= 293, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _phi, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_phi.h"
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    /* φ(n) fits in seq_t, but we promote it to idx_t for safe multiplication. */
    seq_t phi_n = _phi(n);
    idx_t  phi   = (idx_t)phi_n;

    idx_t k = 1;
    while (true) {
        idx_t val = k * phi + 1;               /* k*φ(n)+1 */
        if (isPrime(val))
            return (seq_t)k;                  /* a(n) = k */
        ++k;
    }
}
