/*
 * A202458  Number of (n+2) X 7 binary arrays with consecutive windows of
 *          three bits considered as a binary number nondecreasing in every
 *          row and column.
 * https://oeis.org/A202458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 + 63*n^6 + 1316*n^5 + 13440*n^4 + 75929*n^3 +
 *            242697*n^2 + 517474*n + 600600) / 840 (valid for all n ≥ 1)
 * Range:     u8 none, u16 none, u32 n <= 16, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;               // n^2
    seq_t n3 = n2 * nn;                // n^3
    seq_t n4 = n3 * nn;                // n^4
    seq_t n5 = n4 * nn;                // n^5
    seq_t n6 = n5 * nn;                // n^6
    seq_t n7 = n6 * nn;                // n^7

    seq_t sum = n7
        + (seq_t)63 * n6
        + (seq_t)1316 * n5
        + (seq_t)13440 * n4
        + (seq_t)75929 * n3
        + (seq_t)242697 * n2
        + (seq_t)517474 * nn
        + (seq_t)600600;

    return sum / (seq_t)840;
}
