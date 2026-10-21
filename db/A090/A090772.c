/*
 * A090772  Numbers that are congruent to {2, 8} mod 10.
 * https://oeis.org/A090772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n-3 if n is odd, a(n) = 5*n-2 if n is even (OEIS
 *            A090772)
 * Range:     u8 n <= 51, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;               /* work in seq_t type */
    if (n & 1)                         /* n odd? */
        return 5 * m - 3;              /* a(odd) = 5n-3 */
    else
        return 5 * m - 2;              /* a(even)= 5n-2 */
}
