/*
 * A073464  a(n) = phi(n) mod PrimePi(n).
 * https://oeis.org/A073464
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) mod pi(n) (offset 2)
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 108 terms through n = 9598 (2026-11-01)
 * Helpers:   prime_pi, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/prime_pi.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* For indices below the official offset, return 0 as a placeholder. */
    if (n < 2)
        return (seq_t)0;

    seq_t phi_val = totient(n);          /* Euler's totient of n */
    seq_t pi_val  = prime_pi(n);   /* Prime counting function π(n) */

    /* π(n) ≥ 1 for n ≥ 2, so the modulo operation is safe. */
    return phi_val % pi_val;
}
