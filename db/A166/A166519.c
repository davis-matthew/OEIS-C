/*
 * A166519  a(n) = 1 + 2*(-1)^n + 2*n.
 * https://oeis.org/A166519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*(-1)^n + 2*n
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 1 + 2*n */
    seq_t val = 1 + ((seq_t)n << 1);

    /* Add or subtract 2 depending on parity of n */
    if ((n & 1) == 0)
        val += 2;   /* even n: +2 */
    else
        val -= 2;   /* odd n:  -2 */

    return val;
}
