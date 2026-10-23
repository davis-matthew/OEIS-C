/*
 * A136768  n! never ends in this many 0's in base 7.
 * https://oeis.org/A136768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 57*(n/8) - 1 + 8*(n%8)
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handles the offset – the sequence is defined for n >= 1 */
    if (n == 0)
        return 0;

    /* Compute the integer division and remainder of n by 8. */
    const idx_t block   = n / 8;          /* number of complete blocks of 8 */
    const idx_t offset  = n % 8;          /* position inside the current block */

    /* Convert to seq_t before arithmetic to avoid mixed‑type issues. */
    const seq_t k = (seq_t)block;
    const seq_t r = (seq_t)offset;

    /* Apply the closed form derived from the OEIS pattern. */
    return (seq_t)57 * k - (seq_t)1 + (seq_t)8 * r;
}
