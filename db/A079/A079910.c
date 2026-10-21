/*
 * A079910  Solution to the Dancing School Problem with 5 girls and n+5 boys:
 *          f(5,n).
 * https://oeis.org/A079910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(n)=n^5-5*n^4+25*n^3-55*n^2+80*n-46 for n>=2
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 85, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;

    seq_t res = n5
            - (seq_t)5 * n4
            + (seq_t)25 * n3
            - (seq_t)55 * n2
            + (seq_t)80 * nn
            - (seq_t)46;

    return res;
}
