/*
 * A048467  a(n) = T(6,n), array T given by A047858.
 * https://oeis.org/A048467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * (n + 12) - 5 (for n >= 0) Equivalent
 *            recurrence: a(0) = 1, a(1) = 8, a(2) = 23 a(n) = 5*a(n-1) -
 *            8*a(n-2) + 4*a(n-3) for n >= 3
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)23;

    seq_t a0 = (seq_t)1;   /* a(n-3)      */
    seq_t a1 = (seq_t)8;   /* a(n-2)      */
    seq_t a2 = (seq_t)23;  /* a(n-1)      */
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        /* a3 = 5*a2 - 8*a1 + 4*a0  */
        a3 = a2 * (seq_t)5;
        a3 -= a1 * (seq_t)8;
        a3 += a0 * (seq_t)4;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
