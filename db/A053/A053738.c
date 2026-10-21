/*
 * A053738  If k is in sequence then 2*k and 2*k+1 are not (and 1 is in the
 *          sequence); numbers with an odd number of digits in binary.
 * https://oeis.org/A053738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A053738: nth integer whose binary representation has an odd number
   of digits.  The sequence consists of all numbers n with
   bitlen(n) odd.  The nth such number is obtained by
   selecting the smallest odd length L such that
   Σ_{odd ℓ≤L} 2^{ℓ-1} ≥ n, and returning
   2^{L-1} + (n - Σ_{odd ℓ<L} 2^{ℓ-1} - 1).
   (This is equivalent to generating the numbers with odd binary
   lengths in increasing order.) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle the trivial offset: when n==0 the behaviour is unspecified
       but we simply return 0; the sequence is defined for n≥1 */
    if (n == 0)
        return (seq_t)0;

    seq_t cum = 0;          /* cumulative count of numbers with odd length so far */
    idx_t  L   = 1;          /* current odd binary length */

    /* find the block where the nth number lies */
    while (cum + ((seq_t)1 << (L - 1)) < (seq_t)n) {
        cum += ((seq_t)1 << (L - 1));
        L   += 2;            /* next odd length */
    }

    /* offset inside the found block (0‑based) */
    seq_t offset = (seq_t)n - cum - 1;

    /* base number of the block: the smallest number with
       binary length L (which has leading 1 and all lower bits 0) */
    seq_t base = ((seq_t)1 << (L - 1));

    return base + offset;
}
