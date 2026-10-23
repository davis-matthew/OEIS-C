/*
 * A166819  Number of n X 3 1..3 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          increasing order.
 * https://oeis.org/A166819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 9*n^5 + 55*n^4 + 15*n^3 - 56*n^2 + 66*n)/90
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          // n^2
    seq_t n3 = n2 * sn;          // n^3
    seq_t n4 = n3 * sn;          // n^4
    seq_t n5 = n4 * sn;          // n^5
    seq_t n6 = n5 * sn;          // n^6

    seq_t num = n6;
    num += 9 * n5;
    num += 55 * n4;
    num += 15 * n3;
    num -= 56 * n2;
    num += 66 * sn;

    return num / 90;
}
