/*
 * A016878  a(n) = (5*n + 2)^6.
 * https://oeis.org/A016878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 2)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 7, u64 n <= 324, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to keep all arithmetic in seq_t. */
    seq_t x = (seq_t)5 * (seq_t)n + 2;

    /* Compute x^2, then x^3, and finally x^6 = x^3 * x^3. */
    seq_t x2 = x * x;   /* x² */
    seq_t x3 = x2 * x;  /* x³ */
    seq_t res = x3 * x3;/* x⁶ */

    return res;
}
