/*
 * A174881  a(n) = n^n * (n+1)^n.
 * https://oeis.org/A174881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^n) * ((n+1)^n) = ((n*(n+1))^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1, so n must be ≥ 1.  For n==0 we simply return 0
       (the behaviour for indices below the offset is irrelevant for the tests). */
    if (n == 0)
        return 0;

    /* Compute the base (n*(n+1)) as seq_t to preserve precision. */
    seq_t base = (seq_t)n * ((seq_t)n + 1);

    /* Raise the base to the n‑th power.  big_pow implements fast exponentiation
       and works for all supported seq_t sizes, including arbitrary‑precision
       integer types. */
    return big_pow(base, n);
}
