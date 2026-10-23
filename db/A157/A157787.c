/*
 * A157787  a(n) = 8984250*n - 2515920.
 * https://oeis.org/A157787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8984250*n - 2515920
 * Range:     u8 none, u16 none, u32 n <= 478, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast constants to seq_t to avoid overflow in intermediate arithmetic */
    const seq_t c1 = (seq_t)8984250u;
    const seq_t c2 = (seq_t)2515920u;
    return (seq_t)n * c1 - c2;
}
