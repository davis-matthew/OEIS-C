/*
 * A030978  Maximal number of non-attacking knights on an n X n board.
 * https://oeis.org/A030978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(1)=1; a(2)=4; if n even (>2) => n^2/2; if n odd (>1)
 *            => (n^2+1)/2
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)4;

    seq_t N = (seq_t)n;
    if ((n & 1) == 0)          /* n even */
        return (N * N) / (seq_t)2;
    else                        /* n odd */
        return (N * N + (seq_t)1) / (seq_t)2;
}
