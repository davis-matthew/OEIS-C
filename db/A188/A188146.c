/*
 * A188146  Three interleaved 1st-order polynomials: a(3*n) = 1+4*n, a(1+3*n)
 *          = 3+4*n, a(2+3*n) = 1+n.
 * https://oeis.org/A188146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(3*n) = 1+4*n, a(3*n+1) = 3+4*n, a(3*n+2) = 1+n
 * Range:     u8 n <= 191, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0) {
        seq_t k = (seq_t)(n / 3);
        return 1 + 4 * k;
    } else if (r == 1) {
        seq_t k = (seq_t)((n - 1) / 3);
        return 3 + 4 * k;
    } else { /* r == 2 */
        seq_t k = (seq_t)((n - 2) / 3);
        return 1 + k;
    }
}
