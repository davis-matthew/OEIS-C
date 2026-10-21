/*
 * A099279  Squares of A001076.
 * https://oeis.org/A099279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1)+17*a(n-2)-a(n-3) for n≥3 a(0) = 0, a(1) = 1,
 *            a(2) = 16 (same as OEIS A099279 – squares of A001076)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;

    seq_t a0 = (seq_t)0;   /* a(n-3)   */
    seq_t a1 = (seq_t)1;   /* a(n-2)   */
    seq_t a2 = (seq_t)16;  /* a(n-1)   */
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i) {
        a_next = (seq_t)(17) * a2 + (seq_t)(17) * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }

    return a2;
}
