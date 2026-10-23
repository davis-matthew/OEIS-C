/*
 * A020708  Pisot sequences E(4,9), P(4,9).
 * https://oeis.org/A020708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-3) for n >= 3, with a(0)=4, a(1)=9, a(2)=20
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)20;

    seq_t a_nm3 = (seq_t)4;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)9;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)20;  /* a(n-1) */
    seq_t an = a_nm1;         /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        an = 2 * a_nm1 + a_nm3;   /* a(n) = 2*a(n-1) + a(n-3) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}
