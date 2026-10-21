/*
 * A064815  Related to enumeration of finite automata.
 * https://oeis.org/A064815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(2n-3)*a(n-1) + 2*(2n-5)*(n-4)*a(n-2) for n>=4; a(1)=1,
 *            a(2)=1, a(3)=6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for n <= 0, return 0 */
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)1;
    }
    if (n == 3) {
        return (seq_t)6;
    }

    /* Iterative computation using only a(n-1) and a(n-2) */
    seq_t a_nm2 = (seq_t)1;   /* a(2) */
    seq_t a_nm1 = (seq_t)6;   /* a(3) */
    seq_t curr = (seq_t)0;
    idx_t i;

    for (i = 4; i <= n; ++i) {
        seq_t term1 = (seq_t)2 * (seq_t)(2 * i - 3) * a_nm1;
        seq_t term2 = (seq_t)2 * (seq_t)(2 * i - 5) * (seq_t)(i - 4) * a_nm2;
        curr = term1 + term2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }

    return a_nm1;
}
