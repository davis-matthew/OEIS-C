/*
 * A080088  Let r+i*s be the sum of the first-quadrant Gaussian primes
 *          dividing prime(n); sequence gives r values.
 * https://oeis.org/A080088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 2610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080088: sum of r+s of the first‑quadrant Gaussian
   primes dividing the n‑th ordinary prime
   (i.e., a(p) = p for p ≡ 3 (mod 4), otherwise a(p) = a + b
   where p = a² + b² with a ≥ b > 0). */
#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for index 0 */

    seq_t p = (seq_t)get_prime(n);        /* the n‑th ordinary prime */

    if (p == (seq_t)2)                    /* 2 = (1+i)² → r+s = 2 */
        return (seq_t)2;

    /* For prime ≡ 3 (mod 4): it is Gaussian prime on the real axis,
       r+s equals the prime itself. */
    if ((p % (seq_t)4) == (seq_t)3)
        return p;

    /* Prime ≡ 1 (mod 4): it splits as a² + b² with a > b > 0.
       Find the representation and return a + b. */
    seq_t sqrtp = floor_sqrt(p);
    for (seq_t a_val = sqrtp; a_val > (seq_t)0; --a_val) {
        seq_t a2 = a_val * a_val;
        seq_t b2 = p - a2;
        seq_t b_val = floor_sqrt(b2);
        if (b_val > (seq_t)0 && b_val * b_val == b2) {
            /* a_val ≥ b_val > 0 as required. */
            return a_val + b_val;
        }
    }

    /* Should never reach here for prime p ≥ 2. */
    return (seq_t)0;
}
