/*
 * A152734  5 times pentagonal numbers: 5*n*(3*n-1)/2.
 * https://oeis.org/A152734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*n*(3*n-1)/2
 * Range:     u8 n <= 6, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* cast index to seq_t to avoid loss of precision in intermediate results */
    seq_t tn = (seq_t)n;

    /* compute 5 * n * (3*n - 1) / 2 safely */
    seq_t val = tn * (tn * 3 - 1);
    val = val * 5 / 2;

    return val;
}
