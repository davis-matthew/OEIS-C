/*
 * A154307  a(n) = (n+1)^4*(4+n)!/24.
 * https://oeis.org/A154307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^4*(4+n)!/24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* compute (n+3)! */
    idx_t limit = n + 3;
    for (idx_t i = 2; i <= limit; ++i) {
        result *= (seq_t)i;
    }

    /* divide by 24 */
    result /= (seq_t)24;

    /* multiply by (n+4) */
    result *= ((seq_t)n + (seq_t)4);

    /* compute (n+1)^4 */
    seq_t t = ((seq_t)n + (seq_t)1);
    t *= t;     /* (n+1)^2 */
    t *= t;     /* (n+1)^4 */

    result *= t;

    return result;
}
