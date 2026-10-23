/*
 * A136047  a(1)=1, a(n)=a(n-1)+n if n even, a(n)=a(n-1)+n^2 if n is odd.
 * https://oeis.org/A136047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (n if n is even else n^2), with a(1)=1
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* safety, not used by the exercise */
    seq_t sum = 1;                /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i)
    {
        if ((i & 1) == 0)         /* even i: add i */
            sum += (seq_t)i;
        else                      /* odd i: add i^2 */
        {
            seq_t ii = (seq_t)i;
            sum += ii * ii;
        }
    }
    return sum;
}
