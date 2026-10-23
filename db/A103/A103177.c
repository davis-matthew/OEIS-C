/*
 * A103177  a(n) = (7*3^n + 2n + 5)/4.
 * https://oeis.org/A103177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)-7*a(n-2)+3*a(n-3) with initial values a(0)=3,
 *            a(1)=7, a(2)=18
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)18;

    seq_t a_nm3 = (seq_t)3;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)7;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)18;  /* a(n-1)  */
    seq_t new_val;

    for (idx_t i = 3; i <= n; ++i) {
        new_val = a_nm1 * (seq_t)5;          /* 5 * a(n-1)      */
        new_val += a_nm3 * (seq_t)3;         /* + 3 * a(n-3)    */
        new_val -= a_nm2 * (seq_t)7;         /* - 7 * a(n-2)    */
        a_nm3 = a_nm2;   /* shift window */
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }

    return a_nm1;
}
