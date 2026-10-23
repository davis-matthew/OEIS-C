/*
 * A152271  a(n)=1 for even n and (n+1)/2 for odd n.
 * https://oeis.org/A152271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for even n, a(n) = (n+1)/2 for odd n (OEIS A152271)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {          /* even n */
        return (seq_t)1;
    } else {                     /* odd n */
        return ((seq_t)n + 1) / 2;
    }
}
