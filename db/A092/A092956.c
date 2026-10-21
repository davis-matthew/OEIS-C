/*
 * A092956  a(n) = (2*n+2)!/((n+2)*n!).
 * https://oeis.org/A092956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+2)!/((n+2)*n!) = ∏_{k=n+1}^{2n+2} k / (n+2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A092956.  The calculation is
   performed by a simple multiplicative product over the range
   [n+1, 2n+2] skipping the factor (n+2).  All arithmetic is
   done in the type seq_t, which may be an arbitrary-precision
   integer type. */
seq_t a(idx_t n)
{
    seq_t  res  = 1;              /* running product */
    idx_t  start = n + 1;         /* first factor */
    idx_t  end   = 2*n + 2;       /* last factor */
    idx_t  skip  = n + 2;         /* factor to omit */

    for (idx_t v = start; v <= end; ++v)
    {
        if (v == skip) continue;          /* skip the (n+2) term */
        res *= (seq_t)v;                  /* multiply by current factor */
    }
    return res;
}
