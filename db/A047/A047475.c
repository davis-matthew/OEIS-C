/*
 * A047475  Numbers that are congruent to {1, 2, 3} mod 8.
 * https://oeis.org/A047475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 8*floor((n-1)/3) + ((n-1) mod 3)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 according to the offset of the sequence */
    seq_t t = (seq_t)(n - 1);     /* safe cast; n-1 fits in idx_t */
    seq_t q = t / 3;              /* floor((n-1)/3) */
    seq_t r = t % 3;              /* (n-1) mod 3 */
    return 1 + 8 * q + r;
}
