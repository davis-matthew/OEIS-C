/*
 * A027928  a(n) = T(n, 2*n-5), T given by A027926.
 * https://oeis.org/A027928
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n^4 - 8*n^3 + 39*n^2 - 92*n + 180)/120
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 101, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined – return 0 */
    if (n < 3) return 0;

    seq_t sn  = (seq_t)n;
    seq_t sn2 = sn * sn;
    seq_t sn3 = sn2 * sn;
    seq_t sn4 = sn3 * sn;

    seq_t t2 = sn4 - 8 * sn3 + 39 * sn2 - 92 * sn + 180;
    seq_t t1 = sn - 2;

    return (t1 * t2) / 120;
}
