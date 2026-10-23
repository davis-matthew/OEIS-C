/*
 * A017598  a(n) = (12*n + 6)^6.
 * https://oeis.org/A017598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 6)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 134, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base 12*n + 6 using the idx_t type to avoid
       overflow on the intermediate multiplication. */
    idx_t temp = n * 12;
    temp += 6;

    /* Convert to seq_t for final computation. */
    seq_t base = (seq_t)temp;

    /* Raise to the 6th power by repeated multiplication. */
    seq_t result = 1;
    for (int i = 0; i < 6; ++i)
        result *= base;

    return result;
}
