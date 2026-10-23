/*
 * A131452  a(3n)=4n, a(3n+1)=4n+2, a(3n+2)=4n+1.
 * https://oeis.org/A131452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*n) = 4*n; a(3*n+1) = 4*n+2; a(3*n+2) = 4*n+1
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute division and remainder by 3 */
    idx_t q = n / 3;
    idx_t r = n % 3;
    /* compute result in seq_t, avoiding overflow of idx_t */
    seq_t base = (seq_t)q * (seq_t)4;
    if (r == 0) {
        return base;
    } else if (r == 1) {
        return base + (seq_t)2;
    } else { /* r == 2 */
        return base + (seq_t)1;
    }
}
