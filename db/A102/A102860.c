/*
 * A102860  Number of ways to change three non-identical letters in the word
 *          aabbccdd..., where there are n types of letters.
 * https://oeis.org/A102860
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8 * n * (n-1) * (n-2) / 3
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 1173, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)          /* a(2) = 0, a(1) and below are not defined */
        return 0;
    /* cast to seq_t before multiplication to preserve precision */
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn - 1) * (nn - 2); /* n*(n-1)*(n-2) */
    val /= 3;          /* divide by 3 first to keep numbers smaller */
    val *= 8;          /* multiply by 8 */
    return val;
}
