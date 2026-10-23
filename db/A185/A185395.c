/*
 * A185395  a(3n) = n^2, a(3n+1) = a(3n+2) = 3*n*(n+1)/2.
 * https://oeis.org/A185395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3n) = n^2, a(3n+1) = a(3n+2) = 3*n*(n+1)/2 Offset: 0
 * Range:     u8 n <= 39, u16 n <= 627, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* divide n by 3 to get the group index `q` */
    idx_t q_idx = n / 3;
    idx_t r = n % 3;          /* remainder 0,1,2 */

    seq_t q = (seq_t)q_idx;   /* cast to seq_t */

    if (r == 0) {
        /* a(3q) = q^2 */
        return q * q;
    } else {
        /* a(3q+1) = a(3q+2) = 3*q*(q+1)/2 */
        return ((seq_t)3 * q * (q + 1)) / 2;
    }
}
