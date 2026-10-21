/*
 * A068019  Composite n such that both 1 + phi(n) and -1 + phi(n) are primes,
 *          i.e., phi(n) is the middle term between twin primes (A014574).
 * https://oeis.org/A068019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite integer n such that φ(n)-1 and φ(n)+1
 *            are prime
 * Range:     u8 n <= 49, u16 n <= 4190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* the sequence is defined for n ≥ 1 */
    idx_t count = 0;                        /* number of qualifying terms found so far */
    uint64_t m = 2;                          /* candidate number to test */

    for (;;) {
        /* Skip primes – we need only composite n. */
        if (!is_prime(m)) {
            /* Compute φ(n).  For the sizes we need φ will fit in 64 bits. */
            seq_t phi_val = phi(m);
            uint64_t phi = (uint64_t)phi_val;

            /* Check the twin‑prime condition on φ(n). */
            if (is_prime(phi - 1) && is_prime(phi + 1)) {
                ++count;
                if (count == n)
                    return (seq_t)m;      /* nth desired composite */
            }
        }
        ++m;                                 /* try next integer */
    }
}
