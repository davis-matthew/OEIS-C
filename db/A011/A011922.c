/*
 * A011922  a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3) with a(0)=1, a(1)=3, and
 *          a(2)=33.
 * https://oeis.org/A011922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3), with * a(0) = 1, a(1) =
 *            3, a(2) = 33
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    seq_t a0 = 1;
    seq_t a1 = 3;
    seq_t a2 = 33;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    /* recurrence */
    const seq_t mult = (seq_t)15;
    idx_t k;
    for (k = 3; k <= n; ++k) {
        seq_t next = mult * a2 - mult * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
