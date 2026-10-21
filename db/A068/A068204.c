/*
 * A068204  Let (x_n, y_n) be n-th solution to the Pell equation x^2 = 14*y^2
 *          + 1. Sequence gives {y_n}.
 * https://oeis.org/A068204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - a(n-2), a(1)=0, a(2)=4. (Recurrence from Pell
 *            equation x²=14y²+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    /* Use the recurrence:
       a(n) = 30*a(n-1) - a(n-2)   for n >= 3. */
    seq_t a_nm2 = (seq_t)0; /* a(1) */
    seq_t a_nm1 = (seq_t)4; /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_n = (seq_t)30 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
