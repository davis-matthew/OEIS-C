/*
 * A027603  a(n) = n^3 + (n+1)^3 + (n+2)^3 + (n+3)^3.
 * https://oeis.org/A027603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + (n+1)^3 + (n+2)^3 + (n+3)^3
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 + 1;
    seq_t n2 = n0 + 2;
    seq_t n3 = n0 + 3;

    seq_t cube0 = n0 * n0 * n0;
    seq_t cube1 = n1 * n1 * n1;
    seq_t cube2 = n2 * n2 * n2;
    seq_t cube3 = n3 * n3 * n3;

    return cube0 + cube1 + cube2 + cube3;
}
