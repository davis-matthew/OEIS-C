/*
 * A054567  a(n) = 4*n^2 - 7*n + 4.
 * https://oeis.org/A054567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 - 7*n + 4
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to avoid unintended
       narrowing during intermediate calculations. */
    seq_t N = (seq_t)n;
    /* Use the formula directly; integer arithmetic truncates as required. */
    seq_t result = 4 * N * N - 7 * N + 4;
    return result;
}
