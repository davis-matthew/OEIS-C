/*
 * A238410  a(n) = floor((3(n-1)^2 + 1)/2).
 * https://oeis.org/A238410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*(n-1)^2 + 1)/2)
 * Range:     u8 n <= 7, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = (6*n^2 - 12*n + 7 + (-1)^n)/4 */
seq_t a(idx_t n)
{
    /* Convert n to seq_t once */
    seq_t ns = (seq_t)n;

    /* Compute 6*n^2 - 12*n + 7 */
    seq_t val = (seq_t)6 * ns * ns - (seq_t)12 * ns + (seq_t)7;

    /* Add or subtract 1 depending on parity of n */
    if (n & 1)          /* n is odd: (-1)^n = -1 */
        val -= (seq_t)1;
    else                /* n is even: (-1)^n = +1 */
        val += (seq_t)1;

    return val / (seq_t)4;
}
