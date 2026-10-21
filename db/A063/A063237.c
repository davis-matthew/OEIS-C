/*
 * A063237  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          86 ).
 * https://oeis.org/A063237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4, a(2)=11, a(3)=17, a(4)=25 a(n) = a(n-1) + a(n-2) -
 *            a(n-3) for n >= 5
 * Range:     u8 n <= 37, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range, not used */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)17;
    if (n == 4) return (seq_t)25;

    /* iterate using the recurrence */
    seq_t a_nm3 = (seq_t)11;   /* a(2) */
    seq_t a_nm2 = (seq_t)17;   /* a(3) */
    seq_t a_nm1 = (seq_t)25;   /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t a_n = a_nm1 + a_nm2 - a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
