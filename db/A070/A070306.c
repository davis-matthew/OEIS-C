/*
 * A070306  a(n) = 2*phi(n)/2^omega(n).
 * https://oeis.org/A070306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*phi(n)/2^omega(n) (omega = number of distinct prime
 *            factors of n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 8803 (2026-11-01)
 * Helpers:   omega, pow2, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/omega.h"
#include "../utils/pow2.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* number of distinct prime factors of n */
    idx_t distinct = (idx_t)omega((uint64_t)n);

    /* φ(n) */
    seq_t phi_n = totient_seq((seq_t)n);

    /* denominator: 2^omega */
    seq_t denom = pow2(distinct);

    /* 2*φ(n)/2^omega */
    return (seq_t)2 * phi_n / denom;
}
