/*
 * A017033  a(n) = (7*n + 4)^5.
 * https://oeis.org/A017033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 11, u64 n <= 1018, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (7*n + 4)^5 using repeated multiplication.
       The intermediate results never overflow seq_t for any n
       where the final result fits in seq_t. */
    seq_t val = (seq_t)7 * n + (seq_t)4;
    seq_t result = val * val * val * val * val; /* val^5 */
    return result;
}
