/*
 * A133337  a(3n) = 0, a(3n+1) = a(3n+2) = 5^n.
 * https://oeis.org/A133337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n) = 0, a(3n+1) = a(3n+2) = 5^n
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is a multiple of 3, return 0 */
    if ((n % (idx_t)3) == 0)
        return 0;

    /* otherwise compute 5^(n/3) */
    idx_t k = n / (idx_t)3;
    seq_t result = 1;
    for (idx_t i = 0; i < k; ++i)
        result *= (seq_t)5;
    return result;
}
