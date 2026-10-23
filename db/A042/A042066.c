/*
 * A042066  Numerators of continued fraction convergents to sqrt(557).
 * https://oeis.org/A042066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(0)=23, a(1)=24; for n≥2, if n%5==0: a(n)=46*a(n-1)+a(n-2),
 *            else a(n)=a(n-1)+a(n-2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)23;
    if (n == 1) return (seq_t)24;

    seq_t a_nm2 = (seq_t)23;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)24;   /* a(n-1) */
    seq_t a_n;
    idx_t i;

    for (i = 2; i <= n; ++i) {
        if ((i % 5) == 0) {
            a_n = (seq_t)46 * a_nm1 + a_nm2;
        } else {
            a_n = a_nm1 + a_nm2;
        }
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
