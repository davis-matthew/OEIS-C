/*
 * A040603  Continued fraction for sqrt(629).
 * https://oeis.org/A040603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=25; a(1)=12; a(2)=1; a(3)=1; a(4)=12; a(5)=50; *
 *            a(n)=a(n-5) for n>=5 (periodic part is [12,1,1,12,50])
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)25;
    /* periodic block for n >= 1 */
    static const seq_t tbl[5] = {12, 1, 1, 12, 50};
    return tbl[(n - 1) % 5];
}
