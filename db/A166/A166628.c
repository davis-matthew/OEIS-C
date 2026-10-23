/*
 * A166628  Totally multiplicative sequence with a(p) = 7p for prime p.
 * https://oeis.org/A166628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * 7^bigomega(n)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* For n >= 1: a(n) = n * 7^Omega(n),
       where Omega(n) is the total number of prime factors
       counted with multiplicity.  The helper functions
       bigomega and big_pow already implement this.  */
    seq_t omega = bigomega(n);
    seq_t pow7  = big_pow((seq_t)7, omega);
    return (seq_t)n * pow7;
}
