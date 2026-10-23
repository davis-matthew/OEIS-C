/*
 * A040620  Continued fraction for sqrt(646).
 * https://oeis.org/A040620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=25; for n>0, a(n)=50 if (n-1)%4==3 else 2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)25;

    idx_t mod = (n - (idx_t)1) % (idx_t)4;  /* (n-1) mod 4 */
    return (mod == (idx_t)3) ? (seq_t)50 : (seq_t)2;
}
