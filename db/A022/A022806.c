/*
 * A022806  a(n) = B(n) + c(n) where B(n) is Beatty sequence [ n*e ] and c is
 *          the complement of B.
 * https://oeis.org/A022806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*e) + floor(n*e/(e-1))
 * Range:     u8 n <= 1, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Beatty sequence for e and its complement, summed. */

seq_t a(idx_t n)
{
    /* Choose an integer denominator D > maximal n (here 10^7 > 10000). */
    const seq_t D        = (seq_t)10000000;           /* 1e7  */
    const seq_t NUM_E    = (seq_t)27182818;           /* floor(e * D)      */
    const seq_t NUM_SDEN = NUM_E - D;                 /* denominator for second floor */

    /* floor(n*e)   -> (n * NUM_E) / D */
    seq_t b = (seq_t)((seq_t)n * NUM_E / D);

    /* floor(n*e/(e-1)) -> (n * NUM_E) / NUM_SDEN  */
    seq_t c = (seq_t)((seq_t)n * NUM_E / NUM_SDEN);

    return b + c;
}
