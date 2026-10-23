/*
 * A012772  Take every 5th term of Padovan sequence A000931, beginning with
 *          the sixth term.
 * https://oeis.org/A012772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 5*a(n+2) - 4*a(n+1) + a(n) initial values: a(0) = 1,
 *            a(1) = 3, a(2) = 12
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)12;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)12;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t an = (seq_t)5 * a2 - (seq_t)4 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
