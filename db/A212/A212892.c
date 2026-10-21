/*
 * A212892  a(n) = n^4/8 if n is even, a(n) = (n^2-1)^2/8 if n is odd.
 * https://oeis.org/A212892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (floor(n^2 / 4))^2 (OEIS A212892)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 430, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n as seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t k = n_seq >> 1;           /* floor(n/2) */
    seq_t q;
    if ((n_seq & 1) == 0) {
        /* Even: floor(n^2/4) = (n/2)^2 */
        q = k * k;
    } else {
        /* Odd: floor(n^2/4) = (n/2) * ((n/2)+1) */
        q = k * (k + 1);
    }
    return (seq_t)2 * q * q;
}
