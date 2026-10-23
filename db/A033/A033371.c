/*
 * A033371  a(n) = floor(51/n).
 * https://oeis.org/A033371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(51/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (outside the defined offset) to avoid division by zero. */
    if (n == 0)
        return 0;
    /* Compute 51/n in the native integer width of idx_t (uint64_t) */
    return (seq_t)(51 / n);
}
