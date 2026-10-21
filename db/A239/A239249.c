/*
 * A239249  Number of n X 1 0..4 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it, modulo 5.
 * https://oeis.org/A239249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-3); a(1)=4, a(2)=13, a(3)=41
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used, offset starts at 1 */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)41;

    seq_t a_nm3 = (seq_t)4;   /* a(i-3)  */
    seq_t a_nm2 = (seq_t)13;  /* a(i-2)  */
    seq_t a_nm1 = (seq_t)41;  /* a(i-1)  */
    seq_t a_i;

    for (idx_t i = 4; i <= n; ++i) {
        a_i = (seq_t)3 * a_nm1 + (seq_t)2 * a_nm3;  /* a(i)  */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }
    return a_nm1;
}
