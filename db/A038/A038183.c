/*
 * A038183  One-dimensional cellular automaton 'sigma-minus' (Rule 90):
 *          000,001,010,011,100,101,110,111 -> 0,1,0,1,1,0,1,0.
 * https://oeis.org/A038183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038183 (Rule 90 / sigma-minus)
   a(0) = 1
   a(n+1) = a(n) XOR (a(n) << 2)   for n >= 0
   (This is the same as a(n+1) = a(n) XOR 4*a(n).) */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)1;          /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        x ^= (x << 2);           /* a(i+1) = a(i) XOR (a(i) << 2) */
    }
    return x;
}
