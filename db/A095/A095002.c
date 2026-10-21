/*
 * A095002  a(n) = 9*a(n-1) - 9*a(n-2) + a(n-3); given a(1) = 1, a(2) = 3,
 *          a(3) = 19.
 * https://oeis.org/A095002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 9*a(n-2) + a(n-3) with initial values a(1)=1,
 *            a(2)=3, a(3)=19
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset (n >= 1). */
    if (n == 0)
        return (seq_t)0;            /* Not defined; return 0 for safety. */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)19;

    /* Iteratively compute the recurrence. */
    seq_t a1 = (seq_t)1;   /* a(n-3)  */
    seq_t a2 = (seq_t)3;   /* a(n-2)  */
    seq_t a3 = (seq_t)19;  /* a(n-1)  */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 9 * a3 - 9 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
