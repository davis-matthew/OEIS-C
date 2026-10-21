/*
 * A218503  q-factorial numbers 5!_q.
 * https://oeis.org/A218503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 1)*(n^2 + n + 1)*(n^3 + n^2 + n + 1)*(n^4 + n^3 +
 *            n^2 + n + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;                  /* convert index to seq_t */
    seq_t t1   = sn + 1;                    /* (n + 1) */

    seq_t n2   = sn * sn;                   /* n^2 */
    seq_t t2   = n2 + sn + 1;               /* n^2 + n + 1 */

    seq_t n3   = n2 * sn;                   /* n^3 */
    seq_t t3   = n3 + n2 + sn + 1;          /* n^3 + n^2 + n + 1 */

    seq_t n4   = n3 * sn;                   /* n^4 */
    seq_t t4   = n4 + n3 + n2 + sn + 1;     /* n^4 + n^3 + n^2 + n + 1 */

    return t1 * t2 * t3 * t4;               /* product of the four terms */
}
