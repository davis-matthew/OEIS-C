/*
 * A223834  Number of n X 4 0..1 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A223834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^4 + 4*n^3 + 31*n^2 + 71*n - 54)/6 for n > 1 with
 *            a(1) = 11
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 11;
    }
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;
    seq_t n3  = n2 * sn;
    seq_t n4  = n3 * sn;
    seq_t numer = 2 * n4 + 4 * n3 + 31 * n2 + 71 * sn - 54;
    return numer / 6;
}
