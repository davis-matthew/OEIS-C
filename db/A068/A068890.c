/*
 * A068890  a(1) = 1; a(n) = smallest nontrivial n-th power with property that
 *          digits alternate in parity.
 * https://oeis.org/A068890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = smallest nontrivial n-th power with digits alternating in
 *            parity. The sequence is conjectured to be finite; the only known
 *            terms are for n = 1 … 8. For n > 8 the term is undefined, so
 *            this implementation returns 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined before the offset */
    if (n == 1) return (seq_t)1;   /* 1^1 = 1   */
    if (n == 2) return (seq_t)4;   /* 2^2 = 4   */
    if (n == 3) return (seq_t)8;   /* 2^3 = 8   */
    if (n == 4) return (seq_t)16;  /* 2^4 = 16  */
    if (n == 5) return (seq_t)32;  /* 2^5 = 32  */
    if (n == 6) return (seq_t)729; /* 3^6 = 729 */
    if (n == 7) return (seq_t)2187;/* 3^7 = 2187*/
    if (n == 8) return (seq_t)256; /* 2^8 = 256 */
    return (seq_t)0;               /* undefined for n > 8 */
}
