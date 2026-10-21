/*
 * A085259  Integer part of the conversion from Celsius to Fahrenheit.
 * https://oeis.org/A085259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(9*n/5 + 32)
 * Range:     u8 n <= 124, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (9*n + 160)/5 using integer arithmetic */
    return (((seq_t)n * (seq_t)9) + (seq_t)160) / (seq_t)5;
}
