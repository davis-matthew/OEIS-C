/*
 * A070685  n^6 mod 45.
 * https://oeis.org/A070685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 45
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the base modulo 45 to keep numbers tiny */
    idx_t r = n % 45UL;
    seq_t x = (seq_t)r;          /* current base value (0 <= x < 45) */
    seq_t res = 1;               /* accumulator for the power */

    /* Compute x^6 modulo 45 */
    for (int i = 0; i < 6; ++i)
        res = (res * x) % 45UL;

    return res;
}
