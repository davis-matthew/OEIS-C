/*
 * A060890  a(n) = n^8 + 1.
 * https://oeis.org/A060890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 0 using 8 sequential multiplications. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;  /* convert index to sequence type */
    seq_t r = x;
    for (int i = 1; i < 8; ++i)
        r *= x;          /* repeated multiplication to get n^8 */
    r += 1;              /* add 1 */
    return r;
}
