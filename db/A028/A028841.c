/*
 * A028841  Iterated sum of digits of n is a Fibonacci number.
 * https://oeis.org/A028841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9 * ((n-1)/5) + mapping[(n-1)%5], where mapping = {1, 2,
 *            3, 5, 8}. This implements the OEIS sequence A028841: numbers
 *            whose iterated sum of decimal digits (digital root) is a
 *            Fibonacci number (1,2,3,5,8)
 * Range:     u8 n <= 143, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, may be useful for idx_t */

static const idx_t fib_roots[5] = {1, 2, 3, 5, 8};

seq_t a(idx_t n) {
    /* n >= 1 as per OEIS offset.  Compute block number and residue. */
    idx_t block   = (n - 1) / 5;          /* number of full 9‑number blocks before n */
    idx_t res     = (n - 1) % 5;          /* position within the block (0..4) */
    seq_t block9  = ((seq_t)block) * 9;   /* 9 * block, cast to seq_t for arithmetic */
    seq_t result  = block9 + (seq_t)fib_roots[res];
    return result;
}
