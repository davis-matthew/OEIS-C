/*
 * A178600  Expansion of the polynomial (1+x^3)*(1+x^11).
 * https://oeis.org/A178600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 1 if n==0 or n==3 or n==11 or n==14, otherwise 0
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The coefficient of x^n in (1+x^3)*(1+x^11). */
    if (n == (idx_t)0 || n == (idx_t)3 ||
        n == (idx_t)11|| n == (idx_t)14)
        return (seq_t)1;
    return (seq_t)0;
}
