/*
 * A060571  Tower of Hanoi: the optimal way to move an even number of disks
 *          from peg 0 to peg 2 or an odd number from peg 0 to peg 1 is on
 *          move n to move disk A001511 from peg A060571 (here) to peg
 *          A060572.
 * https://oeis.org/A060571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n ≤ 2 or n is a power of two a(n) = ( a(n – 2^⌊log₂
 *            n⌋) – (–1)^⌊log₂ n⌋ ) mod 3 otherwise (the same recursion that
 *            generates A060571)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   _a_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage: required function */
seq_t a(idx_t n);

#include <stdint.h>
#include "../utils/_a_recursive.h"

/* Public function as required by the build */
seq_t a(idx_t n)
{
    return _a_recursive(n);
}
