/*
 * A168526  a(n) = n^6*(n + 1)/2.
 * https://oeis.org/A168526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6*(n + 1)/2 (OEIS A168526)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 623, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t result;
    if ((n_seq & 1) == 0) {                /* n is even: divide one n by 2 */
        seq_t half = n_seq / 2;              /* n/2  */
        result = half;
        for (int i = 0; i < 5; ++i)          /* multiply by n five more times  */
            result *= n_seq;
        result *= (n_seq + 1);                /* multiply by (n + 1)            */
    } else {                                 /* n is odd: divide (n + 1) by 2   */
        seq_t half = (n_seq + 1) / 2;         /* (n + 1)/2                     */
        result = half;
        for (int i = 0; i < 6; ++i)          /* multiply by n six times        */
            result *= n_seq;
    }
    return result;
}
