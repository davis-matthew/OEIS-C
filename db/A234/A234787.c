/*
 * A234787  Cubes (with at least two digits) that become squares when their
 *          rightmost digit is removed.
 * https://oeis.org/A234787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1000*n^6
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 300, u128 n <= 300,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;      /* promote the index to seq_t */
    seq_t n2 = sn * sn;      /* n^2 */
    seq_t n3 = n2 * sn;      /* n^3 */
    seq_t n6 = n3 * n3;      /* n^6 */
    return n6 * 1000;        /* 1000·n^6 */
}
