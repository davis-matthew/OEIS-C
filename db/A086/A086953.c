/*
 * A086953  Binomial transform of (-1)^mod(n,3) (A257075).
 * https://oeis.org/A086953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) (a(0) = 1, a(1) = 0, a(2)
 *            = 0)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A086953 (OEIS). */
seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)0; /* a(1) */
    seq_t a2 = (seq_t)0; /* a(2) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)3 * a2 - (seq_t)3 * a1 + (seq_t)2 * a0;
        /* Shift for next iteration: a(n-3) <- a(n-2), a(n-2) <- a(n-1), a(n-1) <- ai */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
