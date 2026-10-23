/*
 * A146507  Numbers congruent to {1, 13} mod 42.
 * https://oeis.org/A146507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 42*floor((n-1)/2) + (n%2?1:13)
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t m = (seq_t)n;
    seq_t h = (m - 1) / 2;
    if ((n & 1) == 0)              /* even */
        return (seq_t)42 * h + (seq_t)13;
    else                           /* odd */
        return (seq_t)42 * h + (seq_t)1;
}
