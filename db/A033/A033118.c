/*
 * A033118  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0.
 * https://oeis.org/A033118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-2) - 8*a(n-3) Initial values: a(1)=1,
 *            a(2)=8, a(3)=65
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)65;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)8;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)65;  /* a(n-1)  */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)8 * a_nm1 + a_nm2 - (seq_t)8 * a_nm3;   /* recurrence */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return a_nm1;
}
