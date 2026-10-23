/*
 * A127932  a(4*n) = 4*n+1, a(4*n+1) = a(4*n+2) = a(4*n+3) = 4*n+4.
 * https://oeis.org/A127932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127932
   a(4*n)   = 4*n + 1
   a(4*n+1) = a(4*n+2) = a(4*n+3) = 4*n + 4
   (computed in O(1) time for any idx_t n) */
seq_t a(idx_t n)
{
    seq_t q = (seq_t)(n >> 2);   /* q = floor(n/4) */
    seq_t r = (seq_t)(n & 3);    /* r = n mod 4 */
    return (r == 0) ? (q << 2) + 1 : (q << 2) + 4;
}
