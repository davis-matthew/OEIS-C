/*
 * A191038  Primes that are squares mod 59.
 * https://oeis.org/A191038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   seq_t a(idx_t n) returns the n-th prime p for which (p|59) = 1
 *            (i.e. p is a quadratic residue modulo 59)
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    idx_t count = 0;               /* number of qualifying primes found */
    uint64_t p = 2;                /* current candidate prime */

    for (;; ) {
        if (is_prime(p)) {
            if (jacobi_symbol(p, 59) == 1) {
                ++count;
                if (count == n)
                    return (seq_t)p;
            }
        }
        /* advance to next odd number (2 is handled once) */
        if (p == 2)
            p = 3;
        else
            p += 2;
    }
}
