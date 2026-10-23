/*
 * A181301  Number of 2-compositions of n having no column with equal entries.
 *          A 2-composition of n is a nonnegative matrix with two rows, such
 *          that each column has at least one nonzero entry and whose entries
 *          sum up to n.
 * https://oeis.org/A181301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=6, a(3)=20. For n≥4 the recurrence is a(n)
 *            = 3*a(n‑1) + a(n‑2) – a(n‑3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)20;

    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)2;   /* a(1)   */
    seq_t a2 = (seq_t)6;   /* a(2)   */
    seq_t a3 = (seq_t)20;  /* a(3)   */
    idx_t i;
    for (i = 4; i <= n; ++i) {
        seq_t next = (seq_t)3 * a3 + a2 - a1;  /* a(i) = 3*a(i-1)+a(i-2)-a(i-3) */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
