/*
 * A188820  Number of n X 5 binary arrays without the pattern 0 1 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 64*n^2 - 48*n (odd n>1) * a(n) = 64*n^2 - 48*n + 9 (even
 *            n) * Special case: a(1) = 32 * These identities are valid for
 *            all n >= 2
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1 in OEIS terms,
     * but we simply return 0 in that case for safety. */
    if (n < 1) return 0;

    /* Special case for the first term */
    if (n == 1) return (seq_t)32;

    /* Work in seq_t to allow arbitrary-precision arithmetic when
     * seq_t is an arbitrary-precision type. */
    seq_t nn = (seq_t)n;

    /* Compute 64 * n^2 - 48 * n */
    seq_t val = (seq_t)64 * nn * nn;
    val -= (seq_t)48 * nn;

    /* Add the offset for even n */
    if ((n & 1) == 0) {
        val += (seq_t)9;
    }

    return val;
}
