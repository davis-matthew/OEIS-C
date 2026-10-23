/*
 * A178208  Number of ways to choose three points in an (n X n)-grid (or
 *          geoplane).
 * https://oeis.org/A178208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = binomial(n^2,3) = n^2*(n^2-1)*(n^2-2)/6
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t nn = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t a1 = nn;
    seq_t b1 = nn - 1;
    seq_t c1 = nn - 2;

    /* divide by 2 */
    if (a1 % 2 == 0) {
        a1 /= 2;
    } else if (b1 % 2 == 0) {
        b1 /= 2;
    } else {
        c1 /= 2;
    }

    /* divide by 3 */
    if (a1 % 3 == 0) {
        a1 /= 3;
    } else if (b1 % 3 == 0) {
        b1 /= 3;
    } else {
        c1 /= 3;
    }

    /* multiply safely */
    seq_t res = a1 * b1;
    res = res * c1;
    return res;
}
