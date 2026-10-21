/*
 * A096026  Numbers k such that (k+j) mod (2+j) = 1 for j from 0 to 8 and
 *          (k+9) mod 11 <> 1.
 * https://oeis.org/A096026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + 2520 * ( n + floor((n-1)/10) )
 * Range:     u8 none, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* number of skipped elements (multiples of 11) before n */
    idx_t skip = (n - 1) / 10;           /* integer division, floor */
    seq_t m = (seq_t)n + (seq_t)skip;    /* corresponding k/2520 coefficient */
    return (seq_t)3 + (seq_t)2520 * m;   /* k = 3 + 2520*m */
}
