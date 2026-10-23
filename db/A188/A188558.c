/*
 * A188558  Number of 7 X n binary arrays without the pattern 0 1 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 - 14*n^6 + 154*n^5 - 560*n^4 + 2989*n^3 + 574*n^2 +
 *            17016*n + 20160)/5040
 * Range:     u8 none, u16 n <= 1, u32 n <= 25, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t nn = (seq_t)n;
    seq_t n1 = nn;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n3 * n1;
    seq_t n5 = n4 * n1;
    seq_t n6 = n5 * n1;
    seq_t n7 = n6 * n1;

    // Compute positive part A = n^7 + 154*n^5 + 2989*n^3 + 574*n^2 + 17016*n + 20160
    seq_t A = n7;
    A += (seq_t)154 * n5;
    A += (seq_t)2989 * n3;
    A += (seq_t)574 * n2;
    A += (seq_t)17016 * n1;
    A += (seq_t)20160;

    // Compute subtractive part B = 14*n^6 + 560*n^4
    seq_t B = (seq_t)14 * n6;
    B += (seq_t)560 * n4;

    return (A - B) / (seq_t)5040;
}
