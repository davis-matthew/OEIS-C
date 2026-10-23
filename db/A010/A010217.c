/*
 * A010217  Continued fraction for sqrt(173).
 * https://oeis.org/A010217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=13; for n>0, a(n)=period[(n-1) % 5] with period =
 *            {6,1,1,6,26}
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[5] = {6, 1, 1, 6, 26};
    if (n == 0)
        return (seq_t)13;
    idx_t idx = (n - 1) % 5;
    return period[idx];
}
