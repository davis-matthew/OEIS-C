/*
 * A196664  Expansion of g.f. (1-5*x)/(1-16*x).
 * https://oeis.org/A196664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 16 * a(n-1) for n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */

    /* Compute 16^(n-1) and then multiply by 11 */
    seq_t val = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        val *= (seq_t)16;          /* multiply by 16 each step */

    val *= (seq_t)11;              /* final multiplication by 11 */
    return val;
}
