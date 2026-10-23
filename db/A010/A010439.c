/*
 * A010439  Squares mod 78.
 * https://oeis.org/A010439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 78*floor((n-1)/28) + r[(n-1)%28] where r is the sorted
 *            list of quadratic residues modulo 78:
 *            0,1,3,4,9,10,12,13,16,22,25,27,30,36,39,40,42,43,48,49,51,52,55,61,64,66,69,75
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Residues modulo 78 sorted increasingly */
    static const seq_t r[28] = {
        0, 1, 3, 4, 9, 10, 12, 13, 16, 22,
        25, 27, 30, 36, 39, 40, 42, 43, 48, 49,
        51, 52, 55, 61, 64, 66, 69, 75
    };

    /* n >= 1; n-1 fits in idx_t */
    idx_t idx  = n - 1;
    idx_t block = idx / 28;
    idx_t pos   = idx % 28;

    /* Compute result: 78*block + r[pos] */
    seq_t val = r[pos] + (seq_t)78 * (seq_t)block;
    return val;
}
