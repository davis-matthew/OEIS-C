/*
 * A152745  5 times hexagonal numbers: 5*n*(2*n-1).
 * https://oeis.org/A152745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*n*(2*n-1)
 * Range:     u8 n <= 5, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;                /* convert index to seq_t */
    seq_t two_n_minus1 = (tn << 1) - (seq_t)1;  /* 2*n - 1 */
    seq_t val = tn * two_n_minus1;        /* n*(2*n-1) */
    val *= (seq_t)5;                      /* multiply by 5 */
    return val;
}
