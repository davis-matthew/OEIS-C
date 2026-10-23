/*
 * A172518  Number of ways to place 3 nonattacking queens on an n X n toroidal
 *          board.
 * https://oeis.org/A172518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n^2*(n-2)*(n-4)*(n^2-6*n+12)/6 if n is even, a(n) =
 *            n^2*(n-1)*(n-3)*(n^2-8*n+18)/6 if n is odd
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // n is at least 1 per OEIS offset
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;               // n^2
    seq_t result;

    if ((n & 1) == 0) {               // n even
        seq_t term1 = tn - (seq_t)2;
        seq_t term2 = tn - (seq_t)4;
        seq_t term3 = n2 - (seq_t)6 * tn + (seq_t)12;
        result = (((n2 * term1) * term2) * term3) / (seq_t)6;
    } else {                          // n odd
        seq_t term1 = tn - (seq_t)1;
        seq_t term2 = tn - (seq_t)3;
        seq_t term3 = n2 - (seq_t)8 * tn + (seq_t)18;
        result = (((n2 * term1) * term2) * term3) / (seq_t)6;
    }

    return result;
}
