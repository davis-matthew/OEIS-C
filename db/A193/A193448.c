/*
 * A193448  a(n) = 4*(5*n^2 - 5*n + 1).
 * https://oeis.org/A193448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(5*n^2 - 5*n + 1)
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to avoid accidental truncation
       when the sequence type is wider than idx_t. */
    seq_t N = (seq_t)n;

    /* Compute 5*n^2 - 5*n + 1 using integer arithmetic only. */
    seq_t val = 5 * N * N - 5 * N + 1;

    /* Multiply by 4 to obtain the final term. */
    return 4 * val;
}
