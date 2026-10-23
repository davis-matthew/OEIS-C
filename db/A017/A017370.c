/*
 * A017370  a(n) = (10*n + 8)^6.
 * https://oeis.org/A017370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 8)^6
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 161, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (10*n + 8) using seq_t arithmetic. */
    seq_t base = (seq_t)n * (seq_t)10 + (seq_t)8;

    /* Raise the base to the 6th power by repeated multiplication. */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 6; ++i)
        result *= base;

    return result;
}
