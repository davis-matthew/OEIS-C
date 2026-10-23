/*
 * A167570  The third right hand column of triangle A167569.
 * https://oeis.org/A167570
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^(n-3)*(n-3)!*(2*n-3)!/(2*n-5)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* below the offset, behaviour arbitrary */
    seq_t result = 1;

    /* compute 4^(n-3)*(n-3)! efficiently */
    for (idx_t i = 1; i <= n - 3; ++i) {
        result *= (seq_t)i;       /* multiply by i */
        result *= (seq_t)4;       /* multiply by 4 */
    }

    /* multiply by the remaining factors (2*n-3)*(2*n-4) */
    result *= (seq_t)(2 * n - 3);
    result *= (seq_t)(2 * n - 4);

    return result;
}
