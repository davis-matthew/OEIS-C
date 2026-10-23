/*
 * A036666  Numbers k such that 5*k + 1 is a square.
 * https://oeis.org/A036666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n is odd) -> ((n-1)/2) * (5*((n-1)/2) + 2) (n is even) →
 *            (n/2) * (5*(n/2) - 2)
 * Range:     u8 n <= 14, u16 n <= 229, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 2;
    if (n & 1) {                     /* n is odd */
        return (seq_t)k * (((seq_t)5 * k) + 2);
    } else {                         /* n is even */
        return (seq_t)k * (((seq_t)5 * k) - 2);
    }
}
