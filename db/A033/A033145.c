/*
 * A033145  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,0.
 * https://oeis.org/A033145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1) + a(n-3) - 9*a(n-4) (n >= 5) * with initial
 *            values: a(1)=1, a(2)=9, a(3)=81, a(4)=730
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)81;
    if (n == 4) return (seq_t)730;

    /* a(n-4), a(n-3), a(n-2), a(n-1) respectively */
    seq_t a_n4 = (seq_t)1;
    seq_t a_n3 = (seq_t)9;
    seq_t a_n2 = (seq_t)81;
    seq_t a_n1 = (seq_t)730;

    seq_t an;
    for (idx_t i = 5; i <= n; ++i) {
        an = 9 * a_n1 + a_n3 - 9 * a_n4;   /* recurrence */
        /* shift for next iteration */
        a_n4 = a_n3;
        a_n3 = a_n2;
        a_n2 = a_n1;
        a_n1 = an;
    }
    return a_n1;
}
