/*
 * A016927  a(n) = (6*n + 1)^7.
 * https://oeis.org/A016927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 1)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 94, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (6*n + 1).  All operations are performed in
       seq_t to respect the target integer type. */
    seq_t base = (seq_t)6 * (seq_t)n + (seq_t)1;

    /* Compute the 7th power by repeated multiplication.  The loop
       runs 6 times, so the result equals base^7. */
    seq_t result = base;
    for (int i = 1; i < 7; ++i) {
        result *= base;
    }
    return result;
}
