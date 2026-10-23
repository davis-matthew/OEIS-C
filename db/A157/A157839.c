/*
 * A157839  a(n) = 1728000*n - 1451760.
 * https://oeis.org/A157839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1728000*n - 1451760
 * Range:     u8 none, u16 none, u32 n <= 2486, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is linear; compute directly */
    return (seq_t)n * (seq_t)1728000 - (seq_t)1451760;
}
