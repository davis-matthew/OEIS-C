/*
 * A156712  Indices of star numbers (A003154) that are also triangular numbers
 *          (A000217).
 * https://oeis.org/A156712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n+3) = 15*a(n+2) - 15*a(n+1) + a(n); a(1)=1, a(2)=7, a(3)=91
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)91;

    seq_t a0 = (seq_t)1;    /* a(n-3) */
    seq_t a1 = (seq_t)7;    /* a(n-2) */
    seq_t a2 = (seq_t)91;   /* a(n-1) */

    for (idx_t k = 4; k <= n; ++k)
    {
        /* next = 15*a2 - 15*a1 + a0;  implemented as 15*a2 + a0 - 15*a1 */
        seq_t next = (seq_t)15 * a2 + a0 - (seq_t)15 * a1;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
