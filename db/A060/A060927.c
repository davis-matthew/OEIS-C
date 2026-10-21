/*
 * A060927  Row sums of triangle A060924 (odd part of bisection of Lucas
 *          triangle).
 * https://oeis.org/A060927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-3), with a(0)=3, a(1)=13, a(2)=65
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        (recurrence derived from the generating function (3-2x)/(1-5x-4x^3)) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)65;

    /* sliding window for a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)3;   /* a(n-3) when i==3 */
    seq_t a_nm2 = (seq_t)13;  /* a(n-2)  (kept only for shift) */
    seq_t a_nm1 = (seq_t)65;  /* a(n-1)  */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)5 * a_nm1 + (seq_t)4 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return a_nm1;
}
