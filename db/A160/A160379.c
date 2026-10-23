/*
 * A160379  Number of ON cells at n-th stage of three-dimensional version of
 *          the cellular automaton A160117 using cubes.
 * https://oeis.org/A160379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(1)=1; a(2n)=46*n^3-57*n^2+57*n-19 for n>=1;
 *            a(2n+1)=46*n^3-51*n^2+57*n-17 for n>=1;
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t k;
    seq_t square, cube;
    seq_t term1, term2;
    seq_t result;

    if ((n & 1) == 0) {            /* even index: n = 2*k, k >= 1 */
        k = (seq_t)(n / 2);
        square = k * k;
        cube = square * k;
        term1 = (seq_t)46 * cube + (seq_t)57 * k;
        term2 = (seq_t)57 * square + (seq_t)19;
        result = term1 - term2;
    } else {                       /* odd index: n = 2*k+1, k >= 1 */
        k = (seq_t)((n - 1) / 2);
        square = k * k;
        cube = square * k;
        term1 = (seq_t)46 * cube + (seq_t)57 * k;
        term2 = (seq_t)51 * square + (seq_t)17;
        result = term1 - term2;
    }

    return result;
}
