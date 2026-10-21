/*
 * A079189  Number of anti-commutative closed binary operations (groupoids) on
 *          a set of order n.
 * https://oeis.org/A079189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^n)*((n^2-n)^((n^2-n)/2))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"

/* Number of anti‑commutative closed binary operations (groupoids)
   on a set of order n.  External linkage as required. */
seq_t a(idx_t n)
{
    /* Handle n = 0 so that 0^0 is treated as 1. */
    seq_t nseq = (seq_t)n;

    /* n^n */
    seq_t term1 = _pow_seq_t(nseq, n);

    /* base = n^2 - n */
    seq_t base = nseq * nseq - nseq;

    /* exponent = (n^2 - n) / 2 */
    idx_t exp = ((idx_t)n * (idx_t)n - n) / 2;

    /* (n^2 - n)^((n^2-n)/2) */
    seq_t term2 = _pow_seq_t(base, exp);

    return term1 * term2;
}
