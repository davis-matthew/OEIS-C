/*
 * A040297  Continued fraction for sqrt(315).
 * https://oeis.org/A040297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=17; for n>=1, let e = v₂(n). Then a(n)=1 if e==0, a(n)=2 if
 *            e==1, a(n)=34 if e>=2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)17;
    idx_t e = 0;
    idx_t t = n;
    while ((t & (idx_t)1) == 0) {
        e++;
        t >>= 1;
    }
    if (e == 0)
        return (seq_t)1;
    if (e == 1)
        return (seq_t)2;
    return (seq_t)34;
}
