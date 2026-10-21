/*
 * A099579  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k, k-1) * 3^(k-1).
 * https://oeis.org/A099579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) - 3*a(n-3) - 9*a(n-4) a(0)=0, a(1)=0,
 *            a(2)=1, a(3)=1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four initial values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)0, a1 = (seq_t)0, a2 = (seq_t)1, a3 = (seq_t)1;
    idx_t i;

    for (i = 4; i <= n; ++i) {
        seq_t next = a3 + 6*a2 - 3*a1 - 9*a0;  /* recurrence */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
