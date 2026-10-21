/*
 * A085536  a(n) = (3n)^(3n).
 * https://oeis.org/A085536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n)^(3*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base 3*n in seq_t type */
    seq_t base = (seq_t)n * (seq_t)3;
    /* Exponent is also 3*n, stored as idx_t for the loop counter */
    idx_t exp = 3 * n;
    /* Start with 1 as the multiplicative identity */
    seq_t result = 1;
    /* Multiply base exp times to get base^exp */
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}
