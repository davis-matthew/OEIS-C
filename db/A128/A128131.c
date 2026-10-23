/*
 * A128131  a(n) = 2^A000096(n)*A128130(n).
 * https://oeis.org/A128131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^(n + C(n+1,2)) * A128130(n) where A128130(n) = 0 if n%4
 *            ∈ {2,3}; A128130(n) = (-1)^(n/4) for n%4=0; A128130(n) =
 *            (-1)^(n/4+1) for n%4=1
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute A128131(n).  External linkage as required. */
seq_t a(idx_t n)
{
    /* Zero for n ≡ 2 or 3 (mod 4) */
    if ((n & 3) == 2 || (n & 3) == 3)
        return 0;

    /* exponent = n + n*(n+1)/2  computed in seq_t to avoid overflow */
    seq_t exp_seq = (seq_t)n * ((seq_t)n + 1) / 2;
    exp_seq += (seq_t)n;          /* exp_seq now holds n + C(n+1,2) */

    /* Compute the power of two */
    seq_t val = pow2((idx_t)exp_seq);  /* exp_seq fits in idx_t for n <= 10000 */

    /* Determine the sign factor */
    seq_t factor;
    if ((n & 3) == 0) {
        /* n%4 == 0: factor = (-1)^(n/4) */
        factor = ((n >> 2) & 1) ? (seq_t)-1 : (seq_t)1;
    } else { /* n%4 == 1 */
        /* n%4 == 1: factor = (-1)^(n/4+1) */
        factor = ((n >> 2) & 1) ? (seq_t)1 : (seq_t)-1;
    }

    return val * factor;
}
