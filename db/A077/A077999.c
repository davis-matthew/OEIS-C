/*
 * A077999  Expansion of (1-x)/(1-2*x-2*x^3).
 * https://oeis.org/A077999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (a(n-1) + a(n-3)), a(0) = 1, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the recurrence directly; no dynamic allocation required. */

seq_t a(idx_t n)
{
    /* handle the first few terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* values of a(n-3), a(n-2), a(n-1) respectively */
    seq_t a_n_3 = (seq_t)1;   /* a(0) */
    seq_t a_n_2 = (seq_t)1;   /* a(1) */
    seq_t a_n_1 = (seq_t)2;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)2 * (a_n_1 + a_n_3);
        a_n_3 = a_n_2;
        a_n_2 = a_n_1;
        a_n_1 = next;
    }
    return a_n_1;
}
