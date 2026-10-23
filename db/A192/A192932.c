/*
 * A192932  Squares of numbers in Perrin sequence.
 * https://oeis.org/A192932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Perrin(n))^2 Perrin(0)=3, Perrin(1)=0, Perrin(2)=2,
 *            Perrin(n) = Perrin(n-2) + Perrin(n-3) for n >= 3
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three values directly */
    if (n == 0) return (seq_t)3 * 3;       /* 9   */
    if (n == 1) return (seq_t)0;           /* 0   */
    if (n == 2) return (seq_t)2 * 2;       /* 4   */

    /* iterate the recurrence to obtain Perrin(n) */
    seq_t p0 = 3;        /* Perrin(0) */
    seq_t p1 = 0;        /* Perrin(1) */
    seq_t p2 = 2;        /* Perrin(2) */
    seq_t p_next;

    for (idx_t i = 3; i <= n; ++i) {
        p_next = p0 + p1;   /* Perrin(i) = Perrin(i-2) + Perrin(i-3) */
        p0 = p1;
        p1 = p2;
        p2 = p_next;
    }

    /* Return the square of Perrin(n) */
    return p2 * p2;
}
