/*
 * A110609  a(n) = n * binomial(2*n, n-1).
 * https://oeis.org/A110609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * binomial(2*n, n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Compute a(n) for non‑negative n.  It is equivalent to
   a(n) = n^2 * Catalan(n).  The sequence starts with
   a(0)=0, a(1)=1, a(2)=8, ...                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Cast n to seq_t only after we have verified
       that n > 0, to avoid an underflow in (n-1).   */
    return (seq_t)n * binom((idx_t)(2 * n), n - 1);
}
