/*
 * A030429  a(n+2) = 7*a(n+1) - 7*a(n) - 9*n; a(n+4) = 9*a(n+3) - 22*a(n+2) +
 *          21*a(n+1) - 7*a(n).
 * https://oeis.org/A030429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 7*a(n+1) - 7*a(n) - 9*n; a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a0 = 1, a1 = 2, a_next;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = (seq_t)(i - 2);
        a_next = 7 * a1 - 7 * a0 - 9 * term;
        a0 = a1;
        a1 = a_next;
    }

    return a1;
}
