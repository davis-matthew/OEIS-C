/*
 * A098181  Two consecutive odd numbers separated by multiples of four,
 *          repeated twice, between them, written in increasing order.
 * https://oeis.org/A098181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*floor(n/4) + (n%4==0?1 : (n%4==1?3 : 4))
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* q = floor(n/4)  (n/4 is integer division) */
    idx_t q = n >> 2;          /* faster than n/4 */
    idx_t r = n & 3;           /* n%4 */
    /* base = 4*q, computed in seq_t to keep arithmetic in the result type */
    seq_t base = (seq_t)q << 2; /* 4 * q  */
    switch (r) {
    case 0: return base + 1;
    case 1: return base + 3;
    default: return base + 4;  /* r == 2 or 3 */
    }
}
