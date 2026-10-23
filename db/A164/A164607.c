/*
 * A164607  a(n) = 4*a(n-1) + 4*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A164607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2), a(0) = 1, a(1) = 10;
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of sequence A164607. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    seq_t a0 = (seq_t)1;          /* a(0) */
    seq_t a1 = (seq_t)10;         /* a(1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)4 * a1 + (seq_t)4 * a0; /* a(i) = 4*a(i-1) + 4*a(i-2) */
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
