/*
 * A010132  Continued fraction for sqrt(34).
 * https://oeis.org/A010132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 5; for n ≥ 1, the terms repeat with period 4: a(1)=1,
 *            a(2)=4, a(3)=1, a(4)=10, then a(5)=1, a(6)=4, a(7)=1, a(8)=10, …
 *            Thus a(n) = periodic array {(1,4,1,10)} for n>0
 * Range:     u8 n <= 9074, u16 n <= 9074, u32 n <= 9074, u64 n <= 9074,
 *            u128 n <= 9074, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)5U;
    /* n ≥ 1: use the (n-1) mod 4 index of the period */
    static const seq_t period[4] = {1U, 4U, 1U, 10U};
    return period[(n - 1) & 3U];
}
