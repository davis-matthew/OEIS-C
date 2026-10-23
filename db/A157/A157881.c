/*
 * A157881  Expansion of 152*x^2 / (-x^3+1443*x^2-1443*x+1).
 * https://oeis.org/A157881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0, a(2) = 152, a(3) = 1443 * a(2) = 219336, a(n) = 1443 *
 *            (a(n-1) - a(n-2)) + a(n-3) for n > 3 (recurrence from OEIS
 *            A157881)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)152;
    if (n == 3) return (seq_t)219336;  /* 1443 * 152 */

    seq_t a1 = (seq_t)0;            /* a(n-3) */
    seq_t a2 = (seq_t)152;          /* a(n-2) */
    seq_t a3 = (seq_t)219336;       /* a(n-1) */
    seq_t next;
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)1443 * (a3 - a2) + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
