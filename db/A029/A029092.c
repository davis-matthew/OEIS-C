/*
 * A029092  Expansion of 1/((1-x)*(1-x^5)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A029092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((71*n^3+75*n^2+54*n+70)/54) -
 *            floor((3*n^3+4*n^2+2*n+2)/5) - floor((5*n^3+4*n^2+3*n+4)/7)
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    seq_t term1 = (71*n3 + 75*n2 + 54*N + 70) / 54;
    seq_t term2 = (3*n3 + 4*n2 + 2*N + 2)  / 5;
    seq_t term3 = (5*n3 + 4*n2 + 3*N + 4)  / 7;

    return term1 - term2 - term3;
}
