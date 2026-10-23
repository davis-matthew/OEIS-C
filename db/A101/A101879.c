/*
 * A101879  a(0) = 1, a(1) = 1, a(2) = 2; for n > 2, a(n) = 5*a(n-1) -
 *          5*a(n-2) + a(n-3).
 * https://oeis.org/A101879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 2; for n > 2, a(n) = 5*a(n-1) -
 *            5*a(n-2) + a(n-3);
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)2;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* next = 5*a2 - 5*a1 + a0 */
        next = (seq_t)5 * a2 - (seq_t)5 * a1 + a0;
        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
