/*
 * A198392  a(n) = (6*n*(3*n+7)+(2*n+13)*(-1)^n+3)/16 + 1.
 * https://oeis.org/A198392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n*(3*n+7)+(2*n+13)*(-1)^n+3)/16 + 1
 * Range:     u8 n <= 2, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;               /* cast index to seq_t */
    seq_t term1 = 6 * nseq * (3 * nseq + 7); /* 6*n*(3*n+7)   */
    seq_t term2 = 2 * nseq + 13;          /* 2*n+13        */
    seq_t comb = term1;

    if (n & 1) {                         /* n odd  -> (-1)^n = -1 */
        comb -= term2;                   /* subtract 2*n+13 */
    } else {                             /* n even -> (+1)   */
        comb += term2;                   /* add 2*n+13 */
    }

    comb += 3;                            /* +3            */
    seq_t res = comb / 16 + 1;           /* divide and add 1 */

    return res;
}
