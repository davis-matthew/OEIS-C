/*
 * A221970  Number of -n..n arrays of length 7 with the sum ahead of each
 *          element differing from the sum following that element by n or
 *          less.
 * https://oeis.org/A221970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (488/45)*n^7 + (1708/45)*n^6 + (2912/45)*n^5 +
 *            (602/9)*n^4 + (2072/45)*n^3 + (952/45)*n^2 + (32/5)*n + 1 All
 *            terms are integers for integer n
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t n4 = n3 * ns;
    seq_t n5 = n4 * ns;
    seq_t n6 = n5 * ns;
    seq_t n7 = n6 * ns;

    seq_t num = 0;
    num += (seq_t)488 * n7;
    num += (seq_t)1708 * n6;
    num += (seq_t)2912 * n5;
    num += (seq_t)3010 * n4;
    num += (seq_t)2072 * n3;
    num += (seq_t)952 * n2;
    num += (seq_t)288 * ns;

    return num / (seq_t)45 + (seq_t)1;
}
