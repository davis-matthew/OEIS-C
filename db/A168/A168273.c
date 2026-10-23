/*
 * A168273  a(n) = 2*n + (-1)^n - 1.
 * https://oeis.org/A168273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + (-1)^n - 1
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t val = (seq_t)n;

    /* Multiply by 2 */
    val = val * 2;

    /* If n is odd, subtract 2 */
    if (n & 1U)
        val = val - 2;

    return val;
}
