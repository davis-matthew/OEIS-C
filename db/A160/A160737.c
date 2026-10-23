/*
 * A160737  4*P_5(n), 4 times the Legendre Polynomial of order 5 at n.
 * https://oeis.org/A160737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(63*n^4-70*n^2+15)/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t term = 63 * n4 - 70 * n2 + 15;
    seq_t result;
    if ((nn & 1) == 0) {
        /* n is even – divide n first to keep product small */
        result = (nn >> 1) * term;
    } else {
        /* n is odd – term is even, divide term first */
        result = nn * (term >> 1);
    }
    return result;
}
