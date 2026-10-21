/*
 * A202443  Number of (n+2) X 6 binary arrays avoiding patterns 001 and 110 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 338, a(n) = (2/3) * (n^4 + 12*n^3 + 59*n^2 + 177*n + 159)
 *            for even n ≥ 2, a(n) = (2/3) * (n^4 + 12*n^3 + 59*n^2 + 183*n +
 *            168) for odd n ≥ 3
 * Range:     u8 none, u16 n <= 12, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)338;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;         // n^2
    seq_t n3 = n2 * nn;          // n^3
    seq_t n4 = n3 * nn;          // n^4

    seq_t poly;
    if ((n & 1ULL) == 0) {       // even n
        poly = n4 + (seq_t)12 * n3 + (seq_t)59 * n2 + (seq_t)177 * nn + (seq_t)159;
    } else {                    // odd n (n > 1)
        poly = n4 + (seq_t)12 * n3 + (seq_t)59 * n2 + (seq_t)183 * nn + (seq_t)168;
    }

    return (poly * (seq_t)2) / (seq_t)3;
}
