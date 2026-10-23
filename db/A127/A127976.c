/*
 * A127976  a(n) = ((6*n + 10)/27)*2^(n-1) + ((-1)^(n-1))*(6*n + 5)/27.
 * https://oeis.org/A127976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ((6*n + 10)/27)*2^(n-1) + ((-1)^(n-1))*(6*n + 5)/27
 * Recurrence: a(1)=1, a(2)=1, a(3)=5, a(4)=9,
 *              a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) - 4*a(n-4)  for n≥5
 */
seq_t a(idx_t n)
{
    /* handle the first four obvious terms */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)1;
    if (n == (idx_t)3) return (seq_t)5;
    if (n == (idx_t)4) return (seq_t)9;

    /* recurrence: keep the last four terms */
    seq_t a0 = (seq_t)1;          /* a(n-4)   */
    seq_t a1 = (seq_t)1;          /* a(n-3)   */
    seq_t a2 = (seq_t)5;          /* a(n-2)   */
    seq_t a3 = (seq_t)9;          /* a(n-1)   */
    seq_t next;

    for (idx_t i = (idx_t)5; i <= n; ++i)
    {
        seq_t sum = a3 * (seq_t)2 + a2 * (seq_t)3;
        seq_t sub = a1 * (seq_t)4 + a0 * (seq_t)4;
        next = sum - sub;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
