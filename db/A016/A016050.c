/*
 * A016050  Inverse of 2041st cyclotomic polynomial.
 * https://oeis.org/A016050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 1 if n mod 2041 < 13, otherwise 0 Sequence is periodic
 *            with period 2041. Offset 0
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n modulo the period 2041.  Note that
       idx_t is an unsigned 64‑bit integer, so the
       remainder fits in 32 bits.  */
    uint64_t r = n % 2041ULL;

    /* The first 13 residues (0..12) give a value of 1,
       all others give 0.  */
    if (r < 13ULL)
        return (seq_t)1;
    else
        return (seq_t)0;
}
