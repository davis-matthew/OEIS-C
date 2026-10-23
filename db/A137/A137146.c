/*
 * A137146  Numbers k such that k and k^2 use only the digits 5, 6, 7 and 8.
 * https://oeis.org/A137146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137146: numbers k such that k and k² use only the digits 5,6,7,8.
   All terms are of the form 7 followed by n 6's (for n ≥ 1).  Hence
   a(1)=76 and for n>1: a(n)=10*a(n-1)+6.  The function builds this value
   iteratively, which works for all supported integer types (uint8_t … 
   unsigned __int128 or arbitrary‑precision) because it only uses +, 
   *, and casting to seq_t. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)7;
    for (idx_t i = 0; i < n; ++i)
        x = x * (seq_t)10 + (seq_t)6;
    return x;
}
