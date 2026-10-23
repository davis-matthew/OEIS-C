/*
 * A194654  Number of ways to place 6 nonattacking kings on an n X n
 *          cylindrical chessboard.
 * https://oeis.org/A194654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/720*n*(n^11 - 135*n^9 + 90*n^8 + 7525*n^7 - 9420*n^6 *
 *            - 216045*n^5 + 378090*n^4 + 3192694*n^3 - 6899520*n^2 * -
 *            19450080*n + 48327120), for n ≥ 7 * Initial values: a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=0, a(5)=125, * a(6)=28930
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;
    if (n == 5) return 125;
    if (n == 6) return 28930;

    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t n5 = n4 * N;
    seq_t n6 = n5 * N;
    seq_t n7 = n6 * N;
    seq_t n8 = n7 * N;
    seq_t n9 = n8 * N;
    seq_t n10 = n9 * N;
    seq_t n11 = n10 * N;

    seq_t poly = n11;
    poly -= (seq_t)135 * n9;
    poly += (seq_t)90 * n8;
    poly += (seq_t)7525 * n7;
    poly -= (seq_t)9420 * n6;
    poly -= (seq_t)216045 * n5;
    poly += (seq_t)378090 * n4;
    poly += (seq_t)3192694 * n3;
    poly -= (seq_t)6899520 * n2;
    poly -= (seq_t)19450080 * N;
    poly += (seq_t)48327120;

    seq_t result = N * poly;
    result /= (seq_t)720;
    return result;
}
