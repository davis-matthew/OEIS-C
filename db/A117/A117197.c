/*
 * A117197  a(n) = (n^3 - 1)^3.
 * https://oeis.org/A117197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - 1)^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to seq_t once */
    seq_t nn = (seq_t)n;

    /* compute n^3 */
    seq_t n3 = nn * nn * nn;

    /* compute (n^3 - 1)^3 */
    seq_t k = n3 - 1;
    return k * k * k;
}
