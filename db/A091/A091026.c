/*
 * A091026  Fifth column (m=6) of array A090452.
 * https://oeis.org/A091026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n^6 + 45*n^5 + 745*n^4 + 5595*n^3 +
 *            18694*n^2 + 22440*n + 7200) / 2880
 * Range:     u8 none, u16 n <= 0, u32 n <= 10, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;                 /* cast index to seq_t early */
    seq_t n2 = x * x;
    seq_t n3 = n2 * x;
    seq_t n4 = n3 * x;
    seq_t n5 = n4 * x;
    seq_t n6 = n5 * x;

    seq_t poly = n6
               + (seq_t)45 * n5
               + (seq_t)745 * n4
               + (seq_t)5595 * n3
               + (seq_t)18694 * n2
               + (seq_t)22440 * x
               + (seq_t)7200;

    seq_t a1 = x + (seq_t)1;
    seq_t a2 = x + (seq_t)2;
    seq_t numer = a1 * a2 * poly;

    return numer / (seq_t)2880;
}
