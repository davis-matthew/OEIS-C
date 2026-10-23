/*
 * A041111  Denominators of continued fraction convergents to sqrt(63).
 * https://oeis.org/A041111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 16*a(n-2) - a(n-4) Initial values: a(0) = 1, a(1) = 1,
 *            a(2) = 15, a(3) = 16 Recurrence derived from the continued
 *            fraction convergents to sqrt(63)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)16;

    seq_t f0 = (seq_t)1;   /* a(n-4)   */
    seq_t f1 = (seq_t)1;   /* a(n-3)   */
    seq_t f2 = (seq_t)15;  /* a(n-2)   */
    seq_t f3 = (seq_t)16;  /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)16 * f2 - f0; /* a(i) = 16*a(i-2) - a(i-4) */
        f0 = f1;
        f1 = f2;
        f2 = f3;
        f3 = next;
    }
    return f3;
}
