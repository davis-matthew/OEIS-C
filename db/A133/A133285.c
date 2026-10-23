/*
 * A133285  Indices of the centered 12-gonal numbers which are also 12-gonal
 *          number, or numbers X such that 120*X^2-120*X+36 is a square.
 * https://oeis.org/A133285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=12, a(n+2)=22*a(n+1)-a(n)-10 (for n>=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for offset 1, but safe */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)12;

    seq_t a1 = (seq_t)1;          /* a(1) */
    seq_t a2 = (seq_t)12;         /* a(2) */
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = (seq_t)22 * a2 - a1 - (seq_t)10;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
