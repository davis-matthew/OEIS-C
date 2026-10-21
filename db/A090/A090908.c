/*
 * A090908  Terms a(k) of A073869 for which a(k) is distinct from a(k-1).
 * https://oeis.org/A090908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n · φ) where φ = (1 + √5)/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses: a(n) = ⌊(n + ⌊n·√5⌋)/2⌋.  The function floor_sqrt
   from the utils library gives ⌊√x⌋ for a seq_t argument.       */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Offset < 1 is unused, but guard against it. */
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;                 /* n as seq_t */

    /* compute floor(n · √5) = ⌊√(5 · n²)⌋ */
    seq_t prod   = nn * nn * (seq_t)5;   /* 5·n² */
    seq_t sqrt_n = floor_sqrt(prod);     /* ⌊√(5·n²)⌋ */

    /* a(n) = ⌊(n + ⌊n·√5⌋)/2⌋ */
    return (nn + sqrt_n) / 2;
}
