/*
 * A230128  The number of multinomial coefficients over partitions with value
 *          equal to 4.
 * https://oeis.org/A230128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1/3)*(n-1)) + floor((1/4)*(n-1)) - floor((1/4)*n)
 * Range:     u8 n <= 337, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t once for efficiency */
    seq_t n_seq = (seq_t)n;
    seq_t n_minus_one = n_seq - 1;          /* n-1, safe since n>=1 for defined terms */

    /* compute floor divisions */
    seq_t term1 = n_minus_one / 3;          /* floor((n-1)/3) */
    seq_t term2 = n_minus_one / 4;          /* floor((n-1)/4) */
    seq_t term3 = n_seq / 4;                /* floor(n/4) */

    return term1 + term2 - term3;
}
