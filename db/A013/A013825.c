/*
 * A013825  a(n) = 2^(5*n + 4).
 * https://oeis.org/A013825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(5*n + 4)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the exponent 5*n + 4 safely. */
    idx_t exp = (idx_t)5 * n + (idx_t)4;

    /* Compute 2^exp by repeated multiplication. */
    seq_t value = (seq_t)1;
    for (idx_t i = (idx_t)0; i < exp; ++i) {
        value *= (seq_t)2;
    }

    return value;
}
