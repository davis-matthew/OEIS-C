/*
 * A027413  If n is an odd prime, a(n) = the number of primitive roots mod n,
 *          otherwise a(n) = the number of k < n divisible by at least one but
 *          not all of the prime factors of n.
 * https://oeis.org/A027413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n=1; if n is an odd prime then a(n)=φ(n‑1);
 *            otherwise a(n) = (n‑1) – φ(n) – floor((n‑1)/rad(n)). Offset:1
 *            (a(1)=0)
 * Range:     u8 n <= 372, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, phi, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/rad.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    /* Odd prime case: number of primitive roots modulo n = φ(n-1) */
    if ((n & 1U) && is_prime(n))
        return phi(n - 1);

    /* Composite case: count k < n having at least one but not all prime
       factors of n.  k not coprime to n: (n-1) - φ(n)
       Subtract those divisible by all prime factors: floor((n-1)/rad(n)). */
    seq_t nn_minus1 = (seq_t)n - 1;
    seq_t phi_n     = phi(n);
    seq_t rad_n     = rad(n);
    seq_t div_all   = nn_minus1 / rad_n;           /* multiples of rad(n) below n */

    return nn_minus1 - phi_n - div_all;
}
