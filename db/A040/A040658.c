/*
 * A040658  Continued fraction for sqrt(685).
 * https://oeis.org/A040658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=26, a(n)=period[(n-1) mod 15] with period
 *            {5,1,3,1,12,3,2,2,3,12,1,3,1,5,52}
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)26;
    static const seq_t period[15] =
    {
        5, 1, 3, 1, 12, 3, 2, 2, 3, 12, 1, 3, 1, 5, 52
    };
    return period[(n - 1) % 15];
}
