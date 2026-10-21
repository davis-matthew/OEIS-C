/*
 * A086192  Tribonacci numbers that start with first three squares.
 * https://oeis.org/A086192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), with a(1)=1, a(2)=4, a(3)=9
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset beginning */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;

    /* iterative computation for n >= 4 */
    seq_t a1 = 1;   /* a(n-3)   */
    seq_t a2 = 4;   /* a(n-2)   */
    seq_t a3 = 9;   /* a(n-1)   */
    seq_t ai;
    idx_t i;

    for (i = 4; i <= n; ++i) {
        ai = a1 + a2 + a3;  /* a(n) */
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
