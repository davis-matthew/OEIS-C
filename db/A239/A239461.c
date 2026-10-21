/*
 * A239461  Concatenation of n^3 and n^2.
 * https://oeis.org/A239461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(n^3, n^2) (i.e., n^3 followed by n^2 in decimal)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^2 and n^3 in seq_t */
    seq_t nsq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t ncub = nsq * (seq_t)n;              /* n^3 */

    /* count decimal digits of n^2 */
    seq_t tmp = nsq;
    int d = 1;
    while (tmp >= 10) {
        tmp /= 10;
        ++d;
    }

    /* compute 10^d */
    seq_t ten_pow = 1;
    for (int i = 0; i < d; ++i)
        ten_pow *= 10;

    /* concatenate: n^3 * 10^d + n^2 */
    return ncub * ten_pow + nsq;
}
