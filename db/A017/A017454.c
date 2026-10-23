/*
 * A017454  a(n) = (11*n + 5)^6.
 * https://oeis.org/A017454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 5)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 147, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 11*n + 5 first as seq_t to keep precision when seq_t is large */
    seq_t x = (seq_t)n;
    x = x * (seq_t)11 + (seq_t)5;

    /* raise x to the 6th power using repeated multiplication */
    seq_t r = x;
    for (idx_t i = 1; i < 6; ++i)
        r = r * x;

    return r;
}
