/*
 * A157731  a(n) = 18522*n - 10248.
 * https://oeis.org/A157731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A157731. Formula: a(n) = 18522*n - 10248,  offset 1 */
seq_t a(idx_t n)
{
    /* Cast n to seq_t before arithmetic to avoid accidental int overflow. */
    return (seq_t)18522 * (seq_t)n - (seq_t)10248;
}
