/*
 * A033398  a(n) = floor(78/n).
 * https://oeis.org/A033398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(78/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the OEIS offset is 1, but guard against it */
    if (n == 0)
        return 0;

    /* If n <= 78, perform the division; otherwise the result is 0 */
    if (n <= (idx_t)78)
        return (seq_t)78 / n;

    return 0;
}
