/*
 * A082898  a(n) = A082895(n)/n, where A082895(n) is the closest number to
 *          sigma(n) which is divisible by n.
 * https://oeis.org/A082898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( ( floor(n/2) + σ(n) ) / n ) where σ(n) is the sum
 *            of all positive divisors of n (A000203)
 * Range:     u8 n <= 83, u16 n <= 9451, u32 n <= 9451, u64 n <= 9451,
 *            u128 n <= 9451, big any n
 * Helpers:   sigma_direct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/sigma_direct.h"

/*  Sequence a(n) defined for n ≥ 1.  */
seq_t a(idx_t n)
{
    /* floor(n/2) */
    seq_t half = (seq_t)(n / 2);
    /* σ(n) */
    seq_t sig = sigma_direct(n);
    /* (half + σ(n)) / n  (truncating division) */
    seq_t numer = half + sig;
    return numer / (seq_t)n;
}
