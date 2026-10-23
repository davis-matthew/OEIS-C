/*
 * A100043  a(n) = (3*n-1)!.
 * https://oeis.org/A100043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n-1)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; behaviour for n == 0 is arbitrary */
    if (n == 0)
        return (seq_t)1;

    /* compute the factorial argument: m = 3*n - 1 */
    idx_t m = n * 3 - 1;

    seq_t result = 1;
    for (idx_t i = 2; i <= m; ++i)
        result *= (seq_t)i;

    return result;
}
