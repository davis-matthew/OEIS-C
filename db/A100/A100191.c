/*
 * A100191  The (1,1)-entry in the 3 X 3 matrix M^n, where M = [1,2,1 / 2,2,0
 *          / 1,0,0].
 * https://oeis.org/A100191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 2*a(n-3), a(1)=1, a(2)=6, a(3)=19
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n<1, but safe */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)19;

    seq_t a1 = (seq_t)1;   /* a(1)   */
    seq_t a2 = (seq_t)6;   /* a(2)   */
    seq_t a3 = (seq_t)19;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 * 3 + a2 * 3 - a1 * 2;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
