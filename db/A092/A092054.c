/*
 * A092054  Base-2 logarithm of the sum of numerator and denominator of the
 *          convergents of the continued fraction expansion [1; 1/2, 1/3, 1/4,
 *          ..., 1/n, ...].
 * https://oeis.org/A092054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + 1 – popcount(n) – popcount(n+1)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the base‑2 logarithm of the sum of numerator and denominator
   of the n‑th convergent of the series [1; 1/2, 1/3, 1/4, ...].  */
/* The formula is derived from A092054:  a(n) = Sum_{k=1..n} A001511(floor((k+1)/2)).
   Since A001511(k) = popcount(k), summing gives a(n) = 2*n + 1 - popcount(n) - popcount(n+1). */

#include <stdint.h>

static inline unsigned int popcnt_uint64(uint64_t x)
{
    return (unsigned int)__builtin_popcountll(x);
}

extern seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    unsigned int pc1 = popcnt_uint64(n);
    unsigned int pc2 = popcnt_uint64(n + 1);

    /* seq_t arithmetic only */
    seq_t res = (seq_t)( (seq_t)2 * x + 1 - (seq_t)pc1 - (seq_t)pc2 );
    return res;
}
