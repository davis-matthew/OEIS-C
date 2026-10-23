/*
 * A182097  Expansion of 1/(1-x^2-x^3).
 * https://oeis.org/A182097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3), with a(0) = 1, a(1) = 0, a(2) = 1
 * Range:     u8 n <= 22, u16 n <= 42, u32 n <= 82, u64 n <= 160,
 *            u128 n <= 318, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* a0 = a(i-3), a1 = a(i-2), a2 = a(i-1) */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)0; /* a(1) */
    seq_t a2 = (seq_t)1; /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = a0 + a1;   /* a(i-2) + a(i-3) */
        a0 = a1;                /* shift window */
        a1 = a2;
        a2 = next;
    }
    return a2; /* a(n) */
}
