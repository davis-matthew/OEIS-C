/*
 * A086700  Euler phi function applied to the triangular numbers.
 * https://oeis.org/A086700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ( n*(n+1)/2 ) where φ = Euler's totient function
 * Range:     u8 n <= 22, u16 n <= 370, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 82 terms through n = 4898 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid 64–bit overflow when forming the triangular number */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;
    /* Now tn fits comfortably in idx_t for n up to about 10^9 (much larger than needed) */
    return totient((idx_t)tn);
}
