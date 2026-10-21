/*
 * A070589  a(n) = n^4 mod 46.
 * https://oeis.org/A070589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 46
 * Range:     u8 n <= 3, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo the period first to keep the numbers small. */
    seq_t x = (seq_t)n % 46;     /* x = n mod 46, 0 <= x < 46 */

    /* Compute x² modulo 46. */
    seq_t r = x * x;
    r = r % 46;

    /* Compute (x²)² = x⁴ modulo 46. */
    r = r * r;
    return r % 46;
}
