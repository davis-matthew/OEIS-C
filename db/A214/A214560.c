/*
 * A214560  Number of 0's in binary expansion of n^2.
 * https://oeis.org/A214560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of 0’s in the binary expansion of n²
 * Range:     u8 n <= 15, u16 n <= 294, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Bare‑bones implementation that works for all allowed seq_t sizes)   */
#include <stdbool.h>

/* Count the number of 1‑bits in a value of type seq_t.  Works for
   any unsigned integer or arbitrary‐precision type that supports the
   usual bitwise operators. */
static unsigned popcount_seq_t(seq_t x)
{
    unsigned count = 0;
    while (x) {
        x &= x - 1;            /* clear lowest set bit */
        ++count;
    }
    return count;
}

/* Return the number of bits required to represent a non‑zero value of
   type seq_t (i.e. floor(log₂(x)) + 1).  For x == 0 the result is 0. */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned bits = 0;
    while (x) {
        x >>= 1;
        ++bits;
    }
    return bits;
}

seq_t a(idx_t n)
{
    /* Special case: 0² has the binary representation “0”, so it
       contains exactly one zero. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute n² in seq_t arithmetic. */
    seq_t sq = (seq_t)n * ((seq_t)n);

    /* Number of bits in sq (no leading zeros). */
    unsigned bits = bitlen_seq_t(sq);

    /* Number of 1‑bits. */
    unsigned ones = popcount_seq_t(sq);

    /* Number of 0‑bits is the difference.  The result is small
       for the typical ranges used by the test harness. */
    unsigned zeros = bits - ones;

    return (seq_t)zeros;
}
