/*
 * A081490  Leading term of n-th row of A081491.
 * https://oeis.org/A081490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 - 9*n + 19)/6 - 1 (equivalently a(n) = (n-2)^2 +
 *            a(n-1) + 1 for n > 1, with a(1)=1)
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                            /* promote index to seq_t */
    seq_t term = (seq_t)2 * nn * nn                 /* 2*n^2 */
               - (seq_t)9 * nn                    /* -9*n */
               + (seq_t)19;                       /* +19 */

    seq_t val = nn * term;                          /* n*(2*n^2 - 9*n + 19) */
    val = val / (seq_t)6;                           /* integer division by 6 */
    val = val - (seq_t)1;                           /* subtract 1 */

    return val;
}
