/*
 * A066104  a(2*n) = 2*n, a(2*n+1) = 4*(n+1).
 * https://oeis.org/A066104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implementation for OEIS A066104:
   a(n) = n                if n is even
          2*(n + 1)        if n is odd
   (equivalently a(2*k) = 2*k, a(2*k+1) = 4*(k+1)) */
seq_t a(idx_t n)
{
    if ((n & 1) == 0)       /* even index */
        return (seq_t)n;    /* a(n) = n */
    else                    /* odd index */
        return (seq_t)2 * ((seq_t)n + 1); /* a(n) = 2*(n+1) */
}
