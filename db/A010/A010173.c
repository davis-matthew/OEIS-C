/*
 * A010173  Continued fraction for sqrt(107).
 * https://oeis.org/A010173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=10; for n>0, a(n)=period[(n-1)%6] with
 *            period={2,1,9,1,2,20}
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)10;
    }
    static const seq_t period[6] = {2, 1, 9, 1, 2, 20};
    return period[(n - 1) % 6];
}
