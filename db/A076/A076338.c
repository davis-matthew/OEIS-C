/*
 * A076338  a(n) = 512*n + 1.
 * https://oeis.org/A076338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 512*n + 1
 * Range:     u8 n <= 0, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first to avoid
       unintended truncation when multiplying. */
    seq_t ns = (seq_t)n;
    return ns * (seq_t)512 + (seq_t)1;
}
