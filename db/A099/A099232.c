/*
 * A099232  a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3), with a(0) = 0, a(1) = 1,
 *          a(2) = 2.
 * https://oeis.org/A099232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3)
   with a(0) = 0, a(1) = 1, a(2) = 2. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)2;   /* a(n-1) */
    seq_t a3;

    idx_t i;
    for (i = 3; i <= n; ++i) {
        a3 = (seq_t)2 * a2 + (seq_t)2 * a1 - (seq_t)3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;   /* a(n) after the loop */
}
