/*
 * A154570  The main diagonal of the successive differences of A154127.
 * https://oeis.org/A154570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=-4, for n>=3: a(n)=a(n-1)+2*a(n-2)
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)-4;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)-4;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = a2 + (seq_t)2 * a1;  /* a(i) = a(i-1) + 2*a(i-2) */
        a1 = a2;
        a2 = next;
    }
    return a2;
}
