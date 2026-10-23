/*
 * A040118  Continued fraction for sqrt(130).
 * https://oeis.org/A040118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=11, a(1)=2, a(2)=2, a(3)=22, a(n)=a(n-3) for n>3
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. for n>=1: a(n)=22 if n%3==0 else 2 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)11;
    }
    /* n>=1 */
    if ((n % 3u) == 0) {
        return (seq_t)22;
    }
    /* n%3==1 or 2 */
    return (seq_t)2;
}
